#!/bin/bash
#
# Copyright 2019 ARM Limited.
# All rights reserved.
#

set -e
set -u
set -o pipefail

tmpfile=$(mktemp /tmp/run.XXX)

cleanup() {
    rm -fr "$tmpfile"
}

#trap cleanup 0

# Use -h for command line options
#   $ ./run.sh -h

# Allow the script to be called from another location using an absolute path
mydir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# All of the functionality is in a common script
includepath=$mydir/../../../_common/scripts

# Compiled SystemC executable, unique to this system
# shellcheck disable=SC2034
SIM="$mydir/m55.x"

# This will be populated in the fm-run.sh
declare app_name

# Run the simulation
# We capture the stdout/stderr into a temporary file whilst showing them in the
# console. tee cannot be piped because it will not pass all the variables to
# the rest of the script
# shellcheck source=../../../../_/common/scripts/fm_run.sh
. "$includepath/fm-run.sh" > >(tee "$tmpfile") 2>&1


if [ "$app_name" == "micro_speech" ] || [ "$app_name" == "img_class" ]; then
    # Import post processing and common script
    # shellcheck source=_common/scripts/resources/common/utils.sh
    . "$mydir/../../../_common/scripts/resources/common/utils.sh"

    # shellcheck source=_common/scripts/resources/app_name/post_processing.sh
    . "$mydir/../../../_common/scripts/resources/$app_name/post_processing.sh"
    
    printf "Post process data from simulation output\n"
    post_process_output "$app_name" "$tmpfile"
    printf "Post processing complete.\n"
fi
