#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/task/Lambda.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/hal/HAL.hpp>

#include <signal.h>

/**
 * This example sets a constant value to a PeripheralOutput (a motor) and
 * logs a PeripheralInput (an encoder) periodically.
 */

double period = 0.1;

class ControlSystem {
 public:
  ControlSystem() : td("Main time domain", period, true), enc1{"enc1"}, motor1{"motor1"}, const1{1.0} {
    motor1.getIn().connect(const1.getOut());
    td.addBlock(enc1);
    td.addBlock(const1);
    td.addBlock(motor1);
    eeros::Executor::instance().add(td);
  };

  eeros::control::PeripheralInput<double> enc1;
  eeros::control::PeripheralOutput<double> motor1;
  eeros::control::Constant<double> const1;
  eeros::control::TimeDomain td;
};



class MySafetyProperties : public eeros::safety::SafetyProperties {
 public:
  MySafetyProperties(eeros::logger::Logger& loggerRef)
    : slRun{"Safety Level RUN"},
      slStop{"Safety Level STOP"},
      seRun{"Goto SL RUN"},
      seStop{"Goto SL STOP"},
      log{loggerRef} {
    addLevel(slRun);
    addLevel(slStop);

    slRun.addEvent(seStop, slStop);
    slStop.addEvent(seRun, slRun);

    setEntryLevel(slRun);
  }

 private:
  eeros::safety::SafetyLevel slRun, slStop;
  eeros::safety::SafetyEvent seRun, seStop;
  eeros::logger::Logger& log;
};

void signalHandler(int signum) {
  eeros::safety::SafetySystem::exitHandler();
  eeros::Executor::stop();
}


int main(int argc, char **argv) {
  using namespace eeros;
  using namespace eeros::logger;
  using namespace eeros::control;
  using namespace eeros::task;
  using namespace eeros::hal;
  using namespace eeros::safety;

  StreamLogWriter streamWriter{std::cout};
  Logger::setDefaultWriter(&streamWriter);
  Logger log{};
  log.info() << "demo started.";

  HAL::instance().readConfigFromFile(&argc, argv);

  signal(SIGINT, signalHandler);

  MySafetyProperties safetyProperties{log};
  SafetySystem ss{safetyProperties, period};
  ControlSystem cs;

  int cycleCounter{};
  bool success{false};

  Lambda lambdafunc{[&](){
    log.info() << "encoder counts: " << cs.enc1.getOut().getSignal();

    if (cs.enc1.getOut().getSignal().getValue() > 31.42) { // 5*2PI
      eeros::Executor::stop();
      success = true;
    } else {
      cycleCounter++;
    }

    if (cycleCounter > 100) { //100*0.2s = 20s
      log.error() << "timeout.";
      eeros::Executor::stop();
    }
  }};

  Periodic periodic{"periodic", 0.2, lambdafunc};

  auto& executor = eeros::Executor::instance();
  executor.setMainTask(ss);
  executor.add(periodic);
  executor.run(); // blocking call

  if (!success) {
    log.error() << "failed. Exit with status 1.";
    return 1;
  }

  log.info() << "Application terminated with success.";
}
