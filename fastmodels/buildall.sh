#!/bin/bash

# builds all of the Fast Model 

function fm_build () {

    # build fm system
    if [ -x "$(command -v simgen)" ]; then
         pushd $1
         ./build.sh
         popd
    fi

}


mydir="$(dirname $0)"
pushd $mydir

# save history to logfile
exec > >(tee -i _buildall.log)
exec 2>&1

# Build fm systems
for dir in * ; do
    cpu=$(basename $dir)
    if [ -d "$dir" -a ${cpu:0:1} != "_" ]; then
        for conf in $cpu ; do
            fm_build "$conf" 
        done
    fi 
done

