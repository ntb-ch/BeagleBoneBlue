#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

. "$script_dir/config.sh.in"

git clone $roboticscape_github_address -o upstream "$roboticscape_dir"
pushd "$roboticscape_dir"
git checkout $roboticscape_github_version
popd


if [ ! -d "$eeros_source_dir" ]; then
	git clone $eeros_github_address -o upstream "$eeros_source_dir"
	pushd "$eeros_source_dir"
	git checkout $eeros_github_version
	popd
fi


if [ ! -z ${bbblue_eeros_source_dir+y} ]; then
	if [ ! -d "$bbblue_eeros_source_dir" ]; then
		git clone $bbb_eeros_github_address -o upstream "$bbblue_eeros_source_dir"
		pushd $bbblue_eeros_source_dir
		git checkout $bbb_eeros_github_version
		popd
	fi
fi
