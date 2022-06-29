#!/bin/bash
#
# Copyright 2019 ARM Limited.
# All rights reserved.
#

# Use -h for command line options
#   $ ./run.sh -h

# Allow the script to be called from another location using an absolute path
mydir="$(dirname $0)"

# All of the functionality is in a common script
includepath=$mydir/../../../_common/scripts

# Compiled SystemC executable, unique to this system
SIM=$mydir/m7.x

# Run the simulation
. $includepath/fm-run.sh
