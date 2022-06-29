#!/bin/bash
#
# This script builds the specified application for the
# specified system using the specified  compiler.
#
# $1 - Application
# $2 - System
# $3 - Compiler
# $4 - Application-specific -D option to be supplied to compiler command line (optional)
#
# Copyright (C) 2021 ARM Limited or its affiliates. All rights reservered.
#
set -e

APP_ID=$1
SYSTEM_ID=$2
COMPILER_ID=$3
APP_DEFINES=$4

# Enter the directory containing this file so it can be run from anywhere
pushd `\dirname ${BASH_SOURCE[0]}` > /dev/null

# Mapping of system to cpu
cpu=`python3 -c "import os, json; print(json.load(open(os.path.join('Systems', '$SYSTEM_ID', 'data.json')))['systems']['$SYSTEM_ID']['cpu'])"`
if [ $? -eq 1 ]; then
    echo "Unable to find CPU data for the system. Please check the README to ensure the system $SYSTEM_ID is supported."
    exit 1
fi

# clean the application built files
make -C Applications/${APP_ID} APP=${APP_ID} SYSTEM=${SYSTEM_ID} COMPILER=${COMPILER_ID} CPU=${cpu} APP_DEFINES="${APP_DEFINES}" clean

# build the application
make -C Applications/${APP_ID} APP=${APP_ID} SYSTEM=${SYSTEM_ID} COMPILER=${COMPILER_ID} CPU=${cpu} APP_DEFINES="${APP_DEFINES}"

# Return to the callers original directory
popd > /dev/null
