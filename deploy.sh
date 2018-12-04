#!/bin/bash

script="$(readlink -f $0)"
script_dir="$(dirname $script)"

. "$script_dir/config.sh.in"


target=${target_username}@${target_IP_address}:${target_application_folder}
list="deploy.txt"
tmp=".tmp-deploy"

if [ ! -e $list ]; then
	echo "file list does not exist: $list"
	exit 1
fi

if [ ! -d $tmp ]; then
	mkdir -p $tmp/bin
	mkdir -p $tmp/lib
	mkdir -p $tmp/tmp/bin
	mkdir -p $tmp/tmp/lib
fi

rm -rf $tmp/tmp
mkdir -p $tmp/tmp/bin
mkdir -p $tmp/tmp/lib

while read line
do
	if [ -z $line ]; then
		continue
	fi
	if [ ! -e $line ]; then
		echo $line " does not exist"
		continue
	fi

	base=$(basename $line)
	src="$line"
	folder=
	if [[ "$line" == *.so* ]]; then
		folder="lib"
	else
		folder="bin"
	fi
	dst="$tmp/$folder/$base"
	tmpdst="$tmp/tmp/$folder/$base"

	if [ $src -nt $dst ]; then
		echo $src
		cp "$src" "$tmpdst"
	else
	echo $src
	fi
done < "$list"

echo "scp with: $target"
scp -r $tmp/tmp/* $target
if [ $? -eq 0 ]; then
	cp -r $tmp/tmp/* $tmp
fi

rm -rf $tmp/tmp
