Copyright 2022-2023 Arm Limited (or its affiliates). All rights reserved.

=======================================================================================
Description
=======================================================================================
This README describes this package and the steps you need to follow to upload custom
software


=======================================================================================
Requirements
=======================================================================================
The following platforms, operating systems, and compilers are supported:

Platform            - Linux x86_64
OS                  - CentOS/RedHat Version 7.4
Python3             - Python 3.8
Arm Compiler        - *See 'Supported By This Package' below*
GCC Cross Compiler  - *See 'Supported By This Package' below*


=======================================================================================
Directory Structure
=======================================================================================
Under package root:

./benchmark-package/Applications/*    Directory containing source code for each of the applications
                                     supported by this application package with a few exceptions (see
                                     more below).
./benchmark-package/Systems/*         Directory containing system-specific files required to build an
                                     application for a specific system. Built application tarballs
                                     can also be found for each system in their respective directories
                                     in this directory.

./benchmark-package/Systems/*/README.txt  README containing supported applications and compilers by
                                     each individual system.
./benchmark-package/sw_options.json   Example software options file. See below for more details.
./benchmark-package/build_app.sh      Helper script to build applications in this package.
./benchmark-package/app_checker.py    Helper script to help users (you!) validate that your custom
                                     application has the required files and builds correctly. See
                                     below for more details.
./license_terms/*                    Directory containing Arm and third party license files


=======================================================================================
Supported By This Package
=======================================================================================
This package contains the source code to build a variety of applications for a variety
of systems with a few compilers. The next section describes how to build the provided
applications as an example.

Some systems allow applications to built with or without including FPU instructions.
To achieve this the application Makefile must explicitly assign NO_FPU=1;  thus, the
application Makefile will not use FPU compiler and linker flags and the application
will not initialize/enable the FPU or any associated Coprocessor registers via the
Coprocessor Access Control Register.  See the individual makefile-system.inc files in
to determine which systems support use of this NO_FPU define.

There are a handful of applications that can not be built using the files provided in
this package, however they have been pre-built for the systems they support. These are
indicated below.

This package also enables you to write and build custom applications for a variety of
systems using the existing ones as examples and/or starting places. See the section on
writing custom applications below for how to do this.

This package supports the following systems, applications, and compilers. Each supported
system can be found in the ./benchmark-package/Systems/<system id>/ directory. This
directory also contains a system-specific README which describes how to build applications
with that system and what applications and compilers are supported. It will also specify
which applications are pre-built.  Pre-built applications, such as CMSIS, currently do not
support being built without FPU.

Each system-specific README will specify both names and identifiers. Please only use the
identifiers when calling scripts or filling out sw_options.json.


=======================================================================================
Guidelines
=======================================================================================
We recommend that you do not modify any files in the Systems directory. These files
have system specific code in them that, if changed, could prevent the system or
application from running properly.


=======================================================================================
Building an existing application for a supported system
=======================================================================================

To compile the application source locally on your machine, make sure you have the
compiler installed(AC6/GCC) and then run the build_app.sh script as follows:
> ./build_app.sh <application> <system> <compiler>

The build_app.sh script takes an application name, a system name, and a compiler. The
options for each of these are listed above in the "Supported By This Package" section.

This step outputs the compiled software executable into the
Systems/<system>/<application>/<compiler>/ directory.


=======================================================================================
Creating a custom application
=======================================================================================

Step 1: Modify the "software_name", "software_description", "valid_systems" and "cloud_compilation"
tags in the sw_options.json file. The "software_name" tag value should have the name of all
applications you'd like to build and upload. For example: "software_name": ["myapp1", "myapp2"].
The "valid_systems" tag value should have the system names you want to run your custom
software on. For example: "valid_systems": ["m0px1_cache", "m55x1-axi4"]. The "cloud_compilation"
is an optional tag, value should have the boolean option true/false. For example:"cloud_compilation":
true or unset, the code will be compiled in the cloud. Therefore, source code is required.
If user don't want to upload source code to the cloud, then set "cloud_compilation": false
users should provide a tgz file.

Step 2: In the Applications directory, create your own copy/copies of the app-template
directory. Name the directory and source file to match your "software_name" value tag
you entered in the json file in step1. For example:
    > cp -r Applications/app-template Applications/myapp1
    > mv Applications/myapp1/app-template.c Applications/myapp1/myapp1.c

Repeat this step for the number of applications you'd like to upload.

Step 3: Modify the myapp1.c file. Enter your benchmark code between the comments "Enter
user defined code here" and "End user defined code here" and save changes.

Step 4: If other files or build options are required, please make sure to update
the Makefile at Applications/myapp1/Makefile appropriately.
To build the application without FPU, uncomment the line which assigns NO_FPU=1.

Step 5: If "cloud_compilation": false, when cloud_compilation is set to false,
add your pre-compiled application in the system directory inside selected system eg:(Systems/m55x1-axi4)
inside the folder named by compiler name. For example: when compiler is AC6 and pre-compiled application
is custom_software this application is included in the folder as below
    * Systems/m55x1-axi4/AC6/custom_software_AC6.tgz
    * Systems/m55x1-axi4/GCC/custom_software_GCC.tgz (for GCC compiler)
Or can also call the python script to do same above steps Eg:
     > python app_checker.py -b


    Repeat this step for the number of applications you'd like to upload.

Step 6(Optional): You can compile your custom software (myapp1) either locally on your
machine or skip this step in which case your application will be compiled once uploaded.

    To compile the application source locally on your machine, make sure you have the
    compiler installed (see Requirements section) and then run the build_app.sh script
    as follows:
        > ./build_app.sh myapp1 m0px1_cache AC6

    The build_app.sh script takes an application name, which is a folder under
    the Applications directory, a system name, which is a folder under the Systems
    directory and a compiler, either AC6/GCC.

    See the above section for more on building applications with the build_app.sh
    script. You can compile other applications (myapp2, etc) in the same package before
    uploading.

Step 7: Run the python script app_checker.py to verify the contents of the
    sw_options.json. Optionally you can specify a -b option to build the
    applications and/or a -t option to tar up benchmark-package directory.

Step 8: Optionally you can tar up your top-level directory and create the upload package using
    > tar -cvzf custom-software.tgz benchmark-package/

Step 9: You're now ready to upload your tar'd up custom software package.
