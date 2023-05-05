#!/usr/bin/env python3

"""
This script checks the sw_options.json file to make sure it
specifies correct application(s), system(s) and compiler(s).
It can also build the application(s) and tar up the software-package directory.

Options:
    --build - to build the application(s) for all systems and compilers specified.
    --tar - to create a custom-software.tgz tar file

Copyright (C) 2023 ARM Limited or its affiliates. All rights reservered.
"""

import os
import sys
import json
import subprocess
import argparse
import tarfile
import tempfile
import logging
import glob
from contextlib import contextmanager
from typing import Dict, List


DEFAULT_DESCRIPTION = "Enter your application descriptions here"


class CallCounter:
    """
    Decorator to determine number of calls for a method.

    Used to track the number of errors in logging.
    """

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


def build_app(app, system, compiler):
    """
    Build the application(s) for the specified system(s) using the specified
    compiler(s).

    Args:
        app: The name of the application to build, such as "my_app1".
        system: The system for which to compile the application, such as "m0px1_cache".
        compiler: The compiler to use to build the application, such as "GCC" or "AC6".

    Returns: The return code of the build action.

    """
    build_cmd = ["./build_app.sh", app, system, compiler]
    log_fname = f"build_{app}_{system}_{compiler}.log"

    logger.info(f"Building '{app}' for '{system}' with '{compiler}'.")

    with open(log_fname, "w") as f:
        result = subprocess.run(build_cmd, stdout=f, stderr=f)

    if result.returncode:
        logger.error(f"Build failed. Check '{log_fname}' for errors.")

    return result.returncode


def is_valid_build(application, system, compiler, software_system_meta_data):
    """
    Determine if the combination of app, system, and compiler is a valid combination
    to perform a build.

    Args:
        application: The application, such as "my_app1".
        system: The system on which the build will take place, such as "m0px1_cache".
        compiler: The compiler with which the build will take place, either
            "GCC" or "AC6".
        software_system_meta_data: The collection of metadata from each system's
            `data.json` file.

    Returns: True if the combination is valid, False otherwise.

    """

    # check that the system is indeed supported
    if system not in software_system_meta_data:
        logger.error(
            f"System '{system}' is not supported by the package. Please check the "
            "README for the supported systems."
        )
        return False

    system_data = software_system_meta_data[system]

    # check that the compiler is valid for the system
    if compiler not in system_data["compilers"]:
        logger.warning(
            f"System '{system}' does not support compiler '{compiler}'. Will not be "
            "able to build applications with this combination."
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
                    f"Source code for user-defined application '{application}' does "
                    "not exist, nor does a pre-built application tarball for this "
                    "system and compiler. If this is a user-defined application, "
                    "please add the source code and/or pre-build the application. If "
                    "this is a standard benchmark, please check the spelling"
                )
                return False

    from AssertionValidator import check_assertions

    # ensure no assertions are violated
    options = {"arguments": {"application": application, "compiler": compiler}}
    result, error = check_assertions(options, system_data["assertions"])
    if not result:
        logger.warning(
            f"System '{system}' with application '{application}' and compiler "
            f"'{compiler}' violates the following "
            f"assertion and will not be built: {error['message']}"
        )
        return False

    return True


def check_systems_valid(systems_dir: str, sw_options_data: Dict) -> bool:
    """
    Determine if all systems in sw_options["valid_systems"] are supported.

    Args:
        systems_dir: A directory representing the root of the systems package.
        sw_options_data: The sw_options.json file as a dictionary.

    Returns: True if all systems are supported. False if any system not supported.

    """
    # check to make sure valid_systems specified exist in the Systems folder
    systems = sw_options_data["valid_systems"]
    systems_valid = True
    if len(systems) == 0:
        logger.error("No system(s) specified")
        return False
    for system in systems:
        if system == "" or not os.path.exists(os.path.join(systems_dir, system)):
            logger.error(
                f"Systems folder '{system}' doesn't exist. "
                "Applications for this system will not be built."
            )
            systems_valid = False
    return systems_valid


def check_apps_valid(app_dir: str, sw_options_data: Dict) -> bool:
    """
    Determine if all apps in sw_options["software_name"] are supported.

    Args:
        app_dir: A directory representing the root of the application package.
        sw_options_data: The sw_options.json file as a dictionary.

    Returns: True if all apps exists. False if any app does not exist.

    """
    apps_valid = True
    # check to make sure software name specified exists in the Applications folder
    software = sw_options_data["software_name"]
    if len(software) == 0:
        logger.error("No application(s) specified")
        return False
    for app in software:
        # check software in Applications directory
        if app == "" or not os.path.exists(os.path.join(app_dir, app)):
            logger.error(
                f"Applications folder '{app}' doesn't exist. If this is not a "
                "pre-built, user-defined app, did you "
                "spell it right or forget to add the source code?"
            )
            apps_valid = False
    return apps_valid


def is_cloud_compile(sw_options_data: Dict) -> bool:
    """
    Determine if a cloud compilation is requested.

    Args:
        sw_options_data: The sw_options.json file as a dictionary.

    Returns: True if cloud compilation is requested. False otherwise.

    """
    return sw_options_data.get("cloud_compilation", True)


def check_precompiled_apps_present(systems_dir: str, sw_options_data: Dict) -> bool:
    """
    Determine if all listed applications have a built application present.

    Any application listed in the sw options must have a .tgz file for every compiler
    and system combination.

    Determine if precompiled application .tgz as required my_app1.axf and
    app_data.json files

    Since this function is called after check_systems_valid, we can assume that all
    systems included are supported.

    Args:
        systems_dir: A directory representing the root of the systems package.
        sw_options_data: The sw_options.json file as a dictionary.

    Returns: True if all applications are present, False otherwise.

    """
    precompiled_apps_present = True

    if len(sw_options_data["valid_systems"]) == 0:
        logger.error("No systems specified")
        return False
    for system in sw_options_data["valid_systems"]:
        for app in sw_options_data["software_name"]:
            for compiler in sw_options_data["valid_compilers"]:
                tgz_path = os.path.join(
                    systems_dir, system, app, compiler, f"{app}_{compiler}.tgz"
                )

                if not os.path.exists(tgz_path):
                    logger.error(
                        f"App {app} compiled source code not found in {system} "
                        "system when cloud_compilation is disabled. "
                        "Please check sw_options.json and also make sure you have put "
                        "the pre-compiled app binaries at the right place. "
                        "You can call this script with a -b flag to build from source."
                    )
                    precompiled_apps_present = False
                else:
                    # Untar .tgz precompiled application check if my_app1.axf
                    with get_unpacked_package(tgz_path) as app_tgz:

                        if not os.path.exists(os.path.join(app_tgz, f"{app}.axf")):
                            logger.error(
                                f"App {app} compiled source is found in {system} "
                                f"but there is no {app}.axf file in {app} compiled "
                                "application when cloud_compilation is disabled. "
                                "Please check sw_options.json and also make sure you "
                                "have put the pre-compiled app binaries with "
                                f"{app}.axf at the right place."
                            )
                            precompiled_apps_present = False

                        if not os.path.exists(os.path.join(app_tgz, "app_data.json")):
                            logger.error(
                                f"App {app} compiled source is found in {system} "
                                f"but there is no app_data.json file in {app} compiled "
                                "application when cloud_compilation is disabled. "
                                "Please check sw_options.json and also make sure you "
                                "have put the pre-compiled app binaries with "
                                "app_data.json at the right place."
                            )
                            precompiled_apps_present = False

    return precompiled_apps_present


def get_package_file_list(root_dir: str, sw_options_data: Dict) -> List[str]:
    """
    Determine the files to add to the final application pacakge.

    If cloud compilation is False, no source files are included.

    Args:
        root_dir: A directory representing the root of the benchmark-package folder.
        sw_options_data: The sw_options.json file as a dictionary.

    Returns: A list of files/directories, relative to `directory`, that will be
        placed in the final package.

    """

    # Add everything except Systems and Applications folder to the tar list
    package_flist = os.listdir(root_dir)

    # If you want to compile the code on the cloud, include the source code of the
    # specified apps
    if not is_cloud_compile(sw_options_data):
        package_flist.remove("Applications")

    return package_flist


@contextmanager
def get_unpacked_package(benchmark_package):
    """
    To get an unpacked benchmark package in a temporary directory.

    Usage:

    with get_unpacked_package(benchmark_package) as package_dir:
        # do operations

    # Package area is automatically cleaned up

    Args:
        benchmark_package: A pathlike object pointing to the benchmark package
            .tgz file.

    Returns: A path to the temporary directory.

    """
    with tempfile.TemporaryDirectory() as temp_dir:
        with tarfile.open(benchmark_package, mode="r:gz") as benchmark_tar:
            benchmark_tar.extractall(temp_dir)
            try:
                yield temp_dir
            finally:
                pass


def app_data_json_valid(systems_dir: str, sw_options_data: Dict) -> bool:
    """
    Determine app_data.json file present in precompiled application with

    valid package_id and package_version, matches with system data.json

    Any application listed in the sw options must have a .tgz file for every compiler
    and system combination.

    Since this function is called after check_precompiled_apps_present, we can assume
    that application is included

    Args:
        systems_dir: A directory representing the root of the systems package.
        sw_options_data: The sw_options.json file as a dictionary.

    Returns: True if all applications are present, False otherwise.

    """
    app_data_json_present = True

    for system in sw_options_data["valid_systems"]:
        for app in sw_options_data["software_name"]:
            for compiler in sw_options_data["valid_compilers"]:
                # To get the meta data from data.json file
                software_meta_data_file = os.path.join(systems_dir, system, "data.json")
                with open(software_meta_data_file) as data_json:
                    software_meta_data = json.load(data_json)
                    package_id = software_meta_data["package_id"]
                    package_version = software_meta_data["package_version"]
                    if len(package_id) and len(package_version) == 0:
                        logger.error("No systems specified")
                        return False
                # Here untar the .tgz file check if app_data.json present
                # And then check if app_data.json as valid package id and package
                # version which matches with data.json information
                tgz_path = os.path.join(
                    systems_dir, system, app, compiler, f"{app}_{compiler}.tgz"
                )
                with get_unpacked_package(tgz_path) as app_tgz:
                    with open(
                        os.path.join(app_tgz, "app_data.json"), "r"
                    ) as app_data_json:
                        app_data = json.load(app_data_json)
                    if (
                        app_data["package_id"] is None
                        or app_data["package_id"] != package_id
                    ):
                        logger.error(
                            "app_data.json file is found but package id "
                            "doesn't  match with sw_options.json package"
                        )
                        app_data_json_present = False

                    if (
                        app_data["package_version"] is None
                        or app_data["package_version"] != package_version
                    ):
                        logger.error(
                            "app_data.json file is found but package version "
                            "doesn't  match with sw_options.json package"
                        )
                        app_data_json_present = False

    return app_data_json_present


def app_checker_main():
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
                "Unable to decode software meta data file as JSON. "
                "Has this file been edited?"
            )
    options_valid = True
    # run basic checks on the sw_options.json file
    with open(sw_options_file, "r") as infile:
        sw_options_json = json.load(infile)

        options_valid &= check_apps_valid("Applications", sw_options_json)
        options_valid &= check_systems_valid("Systems", sw_options_json)

        if not options_valid:
            sys.exit(1)

        # At this point all applications and systems are valid.
        software = sw_options_json["software_name"]
        systems = sw_options_json["valid_systems"]

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
                if desc == "" or desc == DEFAULT_DESCRIPTION:
                    logger.warning("A description of your software is missing.")

    # determine all possible combinations based on the software options provided
    build_options = []
    for app in software:
        for system in systems:
            for compiler in compilers:
                logger.info(
                    f"Found possible combination: '{app}' on "
                    f"'{system}' built with '{compiler}'."
                )
                if is_valid_build(app, system, compiler, software_system_meta_data):
                    logger.info("Combination is valid.")
                    build_options.append(
                        {"application": app, "system": system, "compiler": compiler}
                    )
                else:
                    logger.info("Combination is NOT valid.")

    # verify at least one combination is valid
    if len(build_options) == 0:
        logger.error(
            "No valid combinations of the selected systems, "
            f"applications, and compilers in '{sw_options_file}'."
        )
    else:
        logger.info(
            f"Found {len(build_options)} valid build combinations in "
            f"'{sw_options_file}'"
        )

    # exit if errors
    if logger.error.counter:
        logger.fatal(
            f"{logger.error.counter} error(s) detected in the '{sw_options_file}'"
        )
        sys.exit(logger.error.counter)

    logger.info(f"The '{sw_options_file}' file has no errors.")

    # if build option specified build the specified apps
    if args.build:
        for build_opt in build_options:
            build_app(
                build_opt["application"],
                build_opt["system"],
                build_opt["compiler"],
            )

    # If cloud compilation is false, check all app tar file exists in system folder.
    if not is_cloud_compile(sw_options_json):
        precompiled_apps_present = check_precompiled_apps_present(
            "Systems", sw_options_json
        )
        if not precompiled_apps_present:
            sys.exit(1)

        # If app_data.json present inside precompiled application and
        # metadata package_id and package_version present inside app_data.json
        # in the tar precompliled app
        app_data = app_data_json_valid("Systems", sw_options_json)
        if not app_data:
            sys.exit(1)

    # Tar up the folder
    if not logger.error.counter and args.tar:
        os.chdir("..")
        with tarfile.open("custom-software.tgz", "w:gz") as tgz:
            package_flist = get_package_file_list(
                os.path.join(os.getcwd(), "benchmark-package"), sw_options_json
            )
            for package_file in package_flist:
                tgz.add(os.path.join("benchmark-package", package_file))

        logger.info("Tar file ../custom-software.tgz created.")

    sys.exit(logger.error.counter)


if __name__ == "__main__":
    python_modules = os.path.join(os.path.abspath(os.path.dirname(__file__)), "Scripts")
    sys.path.append(os.path.abspath(python_modules))

    app_checker_main()
