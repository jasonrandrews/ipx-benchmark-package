#!/usr/bin/env python3

#
# This script checks the sw_options.json file to make sure it
# specifies correct application(s), system(s) and compiler(s).
# It can also build the application(s) and tar up the software-package directory.
#
# Options:
#     --build - to build the application(s) for all systems and compilers specified.
#     --tar - to create a custom-software.tgz tar file
#
# Copyright (C) 2021 ARM Limited or its affiliates. All rights reservered.
#
import os
import sys
import json
import subprocess
import argparse
import tarfile
import logging
import glob

python_modules = os.path.join(os.path.abspath(os.path.dirname(__file__)), "Scripts")
sys.path.append(os.path.abspath(python_modules))
from AssertionValidator import check_assertions


default_description = "Enter your application descriptions here"


class CallCounter:
    # Decorator to determine number of calls for a method.
    # Used to track number of errors in logging.
    def __init__(self, method):
        self.method = method
        self.counter = 0

    def __call__(self, *args, **kwargs):
        self.counter += 1
        return self.method(*args, **kwargs)


# create a logger
logging.basicConfig(format="%(levelname)s: %(message)s", level=0)
logger = logging.getLogger("cm_app_checker")
logger.error = CallCounter(logger.error)


def parse_command_line():
    parser = argparse.ArgumentParser(description="Application Checker")
    parser.add_argument(
        "sw_options_file",
        default="sw_options.json",
        nargs="?",
        help="Defaults to sw_options.json if not specified",
    )
    parser.add_argument(
        "-b",
        "--build",
        help="Build the applications",
        action="store_true",
        default=False,
    )
    parser.add_argument(
        "-t",
        "--tar",
        help="Tar up the software-package directory",
        action="store_true",
        default=False,
    )
    return parser.parse_args()


args = parse_command_line()
sw_options_file = args.sw_options_file


# load up the meta data about the software package
software_system_meta_data = {}
for software_meta_data_file in glob.glob(os.path.join("Systems", "*", "data.json")):
    try:
        with open(software_meta_data_file) as f:
            software_meta_data = json.load(f)
            software_system_meta_data.update(software_meta_data["systems"])
    except json.decoder.JSONDecodeError:
        logger.error(
            "Unable to decode software meta data file as JSON. Has this file been edited?"
        )


# build the application(s) for specified system(s) using specified compiler(s)
def build_app(app, system, compiler):
    build_cmd = ["./build_app.sh", app, system, compiler]
    log_fname = f"build_{app}_{system}_{compiler}.log"

    logger.info(f"Building '{app}' for '{system}' with '{compiler}'.")

    with open(log_fname, "w") as f:
        result = subprocess.run(build_cmd, stdout=f, stderr=f)

    if result.returncode:
        logger.error(f"Build failed. Check '{log_fname}' for errors.")

    return result.returncode


# validate the combination of app, system, and compiler
def is_valid_build(application, system, compiler):
    # check that the system is indeed supported
    if system not in software_system_meta_data:
        logger.error(
            f"System '{system}' is not supported by the package. Please check the README for the supported systems."
        )
        return False

    system_data = software_system_meta_data[system]

    # check that the compiler is valid for the system
    if compiler not in system_data["compilers"]:
        logger.warning(
            f"System '{system}' does not support compiler '{compiler}'. Will not be able to build applications with "
            "this combination."
        )
        return False

    # inform the user if the application is a custom application
    if application not in system_data["applications"]:
        if not os.path.exists(os.path.join("Applications", application)):
            if not os.path.exists(
                os.path.join(
                    "Systems",
                    system,
                    application,
                    compiler,
                    f"{application}_{compiler}.tgz",
                )
            ):
                logger.warning(
                    f"Source code for user-defined application '{application}' does not exist, nor does a pre-built "
                    "application tarball for this system and compiler. If this is a user-defined application, please "
                    "add the source code and/or pre-build the application. If this is a standard benchmark, please "
                    "check the spelling"
                )
                return False

    # ensure no assertions are violated
    options = {"arguments": {"application": application, "compiler": compiler}}
    result, error = check_assertions(options, system_data["assertions"])
    if not result:
        logger.warning(
            f"System '{system}' with application '{application}' and compiler '{compiler}' violates the following "
            f"assertion and will not be built: {error['message']}"
        )
        return False

    return True


# run basic checks on the sw_options.json file
with open(sw_options_file, "r") as infile:
    sw_options_json = json.load(infile)

    # check to make sure software name specified exists in the Applications folder
    software = sw_options_json["software_name"]
    if len(software) == 0:
        logger.error("No application(s) specified")
    for app in software:
        # check software in Applications directory
        if app == "" or not os.path.exists(os.path.join("Applications", app)):
            logger.warning(
                f"Applications folder '{app}' doesn't exist. If this is not a pre-built, user-defined app, did you "
                "spell it right or forget to add the source code?"
            )

    # check to make sure valid_systems specified exist in the Systems folder
    systems = sw_options_json["valid_systems"]
    if len(systems) == 0:
        logger.error("No system(s) specified")
    for system in systems:
        if system == "" or not os.path.exists(os.path.join("Systems", system)):
            logger.warning(
                f"Systems folder '{system}' doesn't exist. Applications for this system will not be built."
            )
            systems.remove(system)

    # check for at least one compiler
    compilers = sw_options_json["valid_compilers"]
    if len(compilers) == 0:
        logger.error("No compiler(s) specified")

    # check for empty or default description
    descriptions = sw_options_json["software_description"]
    if len(descriptions) == 0 or len(descriptions) != len(software):
        logger.warning(
            "A description of your software is missing or extra descriptions exist."
        )
    else:
        for desc in descriptions:
            if desc == "" or desc == default_description:
                logger.warning("A description of your software is missing.")

# determine all possible combinations based on the software options provided
build_options = []
for app in software:
    for system in systems:
        for compiler in compilers:
            logger.info(
                f"Found possible combination: '{app}' on '{system}' built with '{compiler}'."
            )
            if is_valid_build(app, system, compiler):
                logger.info("Combination is valid.")
                build_options.append(
                    {"application": app, "system": system, "compiler": compiler}
                )
            else:
                logger.info("Combination is NOT valid.")

# verify at least one combination is valid
if len(build_options) == 0:
    logger.error(
        f"No valid combinations of the selected systems, applications, and compilers in '{sw_options_file}'."
    )
else:
    logger.info(
        f"Found {len(build_options)} valid build combinations in '{sw_options_file}'"
    )

# exit if errors
if logger.error.counter:
    logger.fatal(f"{logger.error.counter} error(s) detected in the '{sw_options_file}'")
    sys.exit(logger.error.counter)

logger.info(f"The '{sw_options_file}' file has no errors.")

# if build option specified build the specified apps
if args.build:
    for built_opt in build_options:
        build_app(
            built_opt["application"],
            built_opt["system"],
            built_opt["compiler"],
        )

if not logger.error.counter and args.tar:
    os.chdir("..")
    with tarfile.open("custom-software.tgz", "w:gz") as tgz:
        tgz.add("software-package")
    logger.info("Tar file ../custom-software.tgz created.")

sys.exit(logger.error.counter)
