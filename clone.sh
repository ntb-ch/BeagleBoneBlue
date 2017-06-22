#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

. "$script_dir/config.sh.in"

if [ ! -d "$eeros_source_dir" ]; then
	git clone https://github.com/eeros-project/eeros-framework.git -o upstream "$eeros_source_dir"
	pushd "$eeros_source_dir"
	git checkout master
	popd
fi

if [ ! -z ${bbblue_eeros_source_dir+y} ]; then
	if [ ! -d "$bbblue_eeros_source_dir" ]; then
		git clone https://github.com/eeros-project/bbblue-eeros.git -o upstream "$bbblue_eeros_source_dir"
	fi
fi
