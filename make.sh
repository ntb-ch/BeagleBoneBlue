#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

. "$script_dir/config.sh.in"


#rm -rf "$build_dir" "$install_dir"

mkdir -p $eeros_build_dir
pushd $eeros_build_dir
cmake -DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
      -DCMAKE_INSTALL_PREFIX=$install_dir \
      -DCMAKE_BUILD_TYPE=Release \
      $eeros_source_dir
make
make install
popd


if [ ! -z ${sim_eeros_source_dir+x} ]; then
  mkdir -p $sim_eeros_build_dir
  pushd $sim_eeros_build_dir
  cmake -DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
        -DCMAKE_INSTALL_PREFIX=$install_dir \
	-DCMAKE_BUILD_TYPE=Release \
	$sim_eeros_source_dir
  make
  make install
  popd
fi


if [ ! -z ${bbblue_eeros_source_dir+x} ]; then
  mkdir -p $bbblue_eeros_build_dir
  pushd $bbblue_eeros_build_dir
  cmake -DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
        -DADDITIONAL_INCLUDE_DIRS="/home/graf/Projects/BBB/Robotics_Cape_Installer/libraries/" \
        -DADDITIONAL_LINK_DIRS="/home/graf/Projects/BBB/Robotics_Cape_Installer/libraries/" \
        -DCMAKE_INSTALL_PREFIX=$install_dir \
	-DCMAKE_BUILD_TYPE=Release \
	$bbblue_eeros_source_dir
  make
  make install
  popd
fi


mkdir -p $application_build_dir
pushd $application_build_dir
cmake	-DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
        -DCMAKE_INSTALL_PREFIX=$install_dir \
	-DCMAKE_BUILD_TYPE=Release \
	$application_source_dir
make
popd


