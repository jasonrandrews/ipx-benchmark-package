#!/bin/bash

# Clean all of the Fast Model to prepare for upload to Arm IP Explorer

mydir="$(dirname $0)"
pushd $mydir

# Clean fm systems
for dir in * ; do
    cpu=$(basename $dir)
    if [ -d "$dir" -a ${cpu:0:1} != "_" ]; then
        for conf in $cpu ; do
            pushd $conf
            ./clean.sh
            popd
        done
    fi 
done

