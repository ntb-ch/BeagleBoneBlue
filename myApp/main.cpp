#include <eeros/logger/Logger.hpp>
#include <eeros/logger/StreamLogWriter.hpp>
#include <eeros/safety/SafetySystem.hpp>
#include <eeros/control/TimeDomain.hpp>
#include <eeros/core/Executor.hpp>
#include <eeros/control/Constant.hpp>
#include <eeros/control/I.hpp>
#include <eeros/control/Switch.hpp>
#include <eeros/task/Lambda.hpp>
#include <eeros/hal/HAL.hpp>
#include <eeros/control/PeripheralOutput.hpp>
#include <eeros/control/PeripheralInput.hpp>
#include <signal.h>

using namespace eeros;
using namespace eeros::safety;
using namespace eeros::logger;
using namespace eeros::control;
using namespace eeros::task;
using namespace eeros::hal;

double period = 0.1;

class ControlSystem {
public:
	ControlSystem() : c1(0.1), c2(-0.8), sw(0), led2("led2"), b2("button2"), m1("motor1"), enc1("enc1") {
		i.getIn().connect(c1.getOut());
		i.setInitCondition(0);
		i.enable();
		sw.getIn(0).connect(i.getOut());
		sw.getIn(1).connect(c2.getOut());
		sw.getOut().getSignal().setName("switch output");
		led2.getIn().connect(b2.getOut());
		m1.getIn().connect(sw.getOut());
	}

	Constant<> c1, c2;
	Switch<> sw;
	I<> i;
	PeripheralInput<bool> b2;
	PeripheralInput<double> enc1;
	PeripheralOutput<bool> led2;
	PeripheralOutput<double> m1;
};

class SafetyPropertiesTest : public SafetyProperties {
public:
	SafetyPropertiesTest(ControlSystem& cs) : 
		slState1("state 1"), slState2("state 2"), 
		seGoto1("switch to state 1"), seGoto2("switch to state 2"), 
		cs(cs) {
			
		// ############ Add levels ############
		addLevel(slState1);
		addLevel(slState2);
		
		// ############ Add events to the levels ############
		slState1.addEvent(seGoto2, slState2, kPublicEvent);
		slState2.addEvent(seGoto1, slState1, kPrivateEvent);
		
		// Define and add level functions
		slState1.setLevelAction([&](SafetyContext* privateContext) {
			if(slState1.getNofActivations() == 1) {
				cs.i.setInitCondition(0);
				cs.sw.switchToInput(0);
				cs.sw.setCondition(0.6, 0.05, 1);
				cs.sw.arm();
			}
		});
		
		slState2.setLevelAction([&](SafetyContext* privateContext) {
			if(slState2.getNofActivations() * period > 3) {
				cs.sw.switchToInput(0);
				cs.i.setInitCondition(0);
				privateContext->triggerEvent(seGoto1);
			}
		});
		
		// Define entry level
		setEntryLevel(slState1);	
	};
	SafetyLevel slState1, slState2;
	SafetyEvent seGoto1, seGoto2;
	ControlSystem& cs;
};

void signalHandler(int signum) {
	Executor::stop();
}

int main(int argc, char **argv){
	signal(SIGHUP, signalHandler);
	signal(SIGINT, signalHandler);
	signal(SIGQUIT, signalHandler);
	signal(SIGKILL, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGPWR, signalHandler);

	StreamLogWriter w(std::cout);
	Logger::setDefaultWriter(&w);
	Logger log;
	
	log.info() << "MyApp started... ";
		
	HAL::instance().readConfigFromFile(&argc, argv);
	eeros::hal::Output<bool>* led = HAL::instance().getLogicOutput("led1");
	bool toggle;
	ControlSystem controlSystem;

	// Create and initialize safety system
	SafetyPropertiesTest ssProperties(controlSystem);
	SafetySystem safetySys(ssProperties, period);
	
	// create time domain and add blocks of control system
	TimeDomain td("td1", period, true);
	td.addBlock(controlSystem.c1);
	td.addBlock(controlSystem.c2);
	td.addBlock(controlSystem.i);
	td.addBlock(controlSystem.sw);
	td.addBlock(controlSystem.b2);
	td.addBlock(controlSystem.led2);
	td.addBlock(controlSystem.m1);
	td.addBlock(controlSystem.enc1);
	controlSystem.sw.registerSafetyEvent(&safetySys, &ssProperties.seGoto2);
	
	// create periodic function for logging
	Lambda l1 ([&] () { });
	Periodic periodic("per1", 0.5, l1);
	periodic.monitors.push_back([&](PeriodicCounter &pc, Logger &log){
		log.info() << controlSystem.sw.getOut().getSignal();
		log.info() << controlSystem.enc1.getOut().getSignal();
		led->set(toggle);
		toggle = !toggle;
	});
	
	// Create and run executor
	auto& executor = eeros::Executor::instance();
	executor.setMainTask(safetySys);
	executor.add(td);
	executor.add(periodic);
	executor.run();

	log.info() << "Test finished...";
}