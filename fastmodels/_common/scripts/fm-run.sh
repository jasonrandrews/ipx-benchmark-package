#!/bin/bash
#
# Copyright 2019 ARM Limited.
# All rights reserved.
#

# Script to run a Fast Model simuation with arguments to load application and plugins
set -e
set -u
set -o pipefail


# allow the script to be called from another location using an absolute path
mydir=$(dirname "$(realpath "$0")")

# Fast Model Plugins
plugins=$PVLIB_HOME/plugins/Linux64_GCC-7.3/

# Location of software programs
sw=$mydir/../../Applications/

# Make sure executable exists
# SIM should be pre-defined
[[ -z "$SIM" || ! -f "$SIM" ]] && echo Error, cannot find simulation executable, make sure to build the Fast Model system using build.sh && exit 0

default_compiler="AC6"

app=""
app_name=""
compiler=""
flag_cadi_debug=""
flag_generic_trace=""
flag_rtl_tarmac=""
flag_tarmac=""

usage() {
    echo "Usage: $0 [-a application] specifies software to run (required) " 1>&2
    echo "          [-c --compiler] specifies the compiler the software was compiled with (defaults to $default_compiler, can also be GCC) " 1>&2
    echo "          [-d --cadi-debug] starts cadi server " 1>&2
    echo "          [-g --generic-trace] loads generic trace plugin " 1>&2
    echo "          [-h --help] prints help message " 1>&2
    echo "          [-r --rtl-tarmac] creates RTL compatible tarmac file " 1>&2
    echo "          [-t --tarmac] enable tarmac trace " 1>&2
    exit 1
}

# Process command line arguments
opts=$(getopt -o a:c:dghrtx --long application:,compiler:,cadi-debug,generic-trace,help,rtl-tarmac,tarmac -n "$(basename "$0")" -- "$@")
eval set -- "$opts"
while [ $# -gt 0 ]; do
  if [ -n "${opt_prev:-}" ]; then
    eval "$opt_prev=\$1"
    opt_prev=
    shift 1
    continue
  elif [ -n "${opt_append:-}" ]; then
    eval "$opt_append=\"\${$opt_append:-} \$1\""
    opt_append=
    shift 1
    continue
  fi
  case "$1" in
    -a | --application)
        opt_prev=app_name
        ;;

    -c | --compiler)
        opt_prev=compiler
        ;;

    -d | --cadi-debug)
        flag_cadi_debug="-S -p"
        ;;

    -g | --generic-trace)
        flag_generic_trace="--plugin $plugins/GenericTrace.so -C TRACE.GenericTrace.trace-sources=READ_ACCESS,WRITE_ACCESS"
        ;;

    -h | --help)
        usage
        exit 0
        ;;

    -r | --rtl-tarmac)
        flag_rtl_tarmac="--plugin $plugins/TarmacText.so"
        ;;

    -t | --tarmac)
        flag_tarmac="--plugin $plugins/TarmacTrace.so"
        ;;

    -x)
        set -x
        ;;

    --)
        shift;
        break 2
        ;;
  esac
  shift 1
done


# specify compiler to use
if [ "$compiler" = "" ]; then
    compiler="$default_compiler"
    echo "Compiler: AC6 (by default)"
else
    echo "Compiler: $compiler"
fi


if [ "$app_name" = "" ]; then
    echo "Warning, no application specified, exiting..."
    exit 1
else
    app_path="$sw/$app_name/$compiler/$app_name.axf"
    if [ ! -f "$app_path" ]; then
        echo "Can't find application $app_path"
        exit 1;
    fi
fi

[ "$*" != "" ] && echo "Adding $* to the command"

echo "Running: $SIM $app $flag_cadi_debug $flag_generic_trace $flag_rtl_tarmac $flag_tarmac --stat $*"

# shellcheck disable=SC2086
$SIM -a $app_path $flag_cadi_debug $flag_generic_trace $flag_rtl_tarmac $flag_tarmac --stat "$@"

