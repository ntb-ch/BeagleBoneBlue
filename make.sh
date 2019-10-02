#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

. "$script_dir/config.sh.in"


mkdir -p $eeros_build_dir
pushd $eeros_build_dir
cmake -DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
      -DCMAKE_INSTALL_PREFIX=$install_dir \
      -DCMAKE_BUILD_TYPE=Release \
      $eeros_source_dir
make
make install
popd


if [ ! -z ${bbblue_eeros_source_dir+x} ]; then
  mkdir -p $bbblue_eeros_build_dir
  pushd $bbblue_eeros_build_dir
  cmake -DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
        -DADDITIONAL_INCLUDE_DIRS="$roboticscape_dir/libraries/" \
        -DADDITIONAL_LINK_DIRS="$roboticscape_dir/libraries/" \
        -DCMAKE_INSTALL_PREFIX=$install_dir \
        -DCMAKE_BUILD_TYPE=Release \
        -DREQUIRED_EEROS_VERSION=$eeros_required_version \
	$bbblue_eeros_source_dir
  make
  make install
  popd
fi


mkdir -p $application_build_dir
pushd $application_build_dir
cmake	-DCMAKE_TOOLCHAIN_FILE=$toolchain_file \
      -DADDITIONAL_INCLUDE_DIRS="$roboticscape_dir/libraries/" \
      -DADDITIONAL_LINK_DIRS="$roboticscape_dir/libraries/" \
      -DCMAKE_INSTALL_PREFIX=$install_dir \
      -DCMAKE_BUILD_TYPE=Release \
      -DREQUIRED_EEROS_VERSION=$eeros_required_version \
      -DREQUIRED_BBB_EEROS_VERSION=$bbb_eeros_required_version \
	$application_source_dir
make
popd
