#!/bin/bash
#
# build.sh - Build the DMA example.
#
# Copyright 2015 ARM Limited.
# All rights reserved.
#

arch=$(uname -m)

if [ "$arch" == 'aarch64' ]; then
    make rel_gcc63_64
    cp $PVLIB_HOME/plugins/Linux64_armv8l_GCC-6.3/newt.so .
else
    make rel_gcc73_64
fi

