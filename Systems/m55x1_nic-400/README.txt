Copyright 2022 Arm Limited (or its affiliates). All rights reserved.

=======================================================================================
Description
=======================================================================================
This README describes the supported compilers and applications for this system.

For details on how to use this information, please see the top level README.txt


=======================================================================================
Supported By This System
=======================================================================================

The names and identifiers of this system and it's supported applications and compilers
are shown below in the format 'name (identifier)'. The names are for informational
purposes only, pleaes use the identifiers in all cases.

Cortex-M55 with FPU (m55x1_nic-400)
    Applications:
        dhrystone (dhrystone)
        hello (hello)
        coremark (coremark)
        whetstone (whetstone)
        matrix-add (matrix-add)
        cmsis-dsp-BasicMaths-F32 (cmsis-dsp-basicmaths-f32) -- pre-built
        cmsis-dsp-BasicMaths-Q31 (cmsis-dsp-basicmaths-q31) -- pre-built
    Compilers:
        GCC, Version 10.3-2021.07 (GCC)
        AC6, Version 6.18 (AC6)
    Assertions:
        When 'application' is 'cmsis-dsp-basicmaths-f32', 'compiler' must be one of ['AC6'].
        When 'application' is 'cmsis-dsp-basicmaths-q31', 'compiler' must be one of ['AC6'].
