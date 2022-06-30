## Arm IP Explorer custom software development

This software is for [Arm IP Explorer](https://ipexplorer.arm.com)

It is useful for preparing a software application for benchmarking on Arm processors.

Please request access to Arm IP Explorer and know that you will need an Arm account. If you need a new account visit [Arm Developer](https://developer.arm.com) and click the person in the top right corner and click Register. 

## Below are the steps you need to follow to create and upload custom software applications

This assumes you are running in a container which includes the required tools or you have installed the tools yourself on Linux.

Required tools are Arm Fast Models and Arm Compiler for Embedded. 

There are multiple ways to develop an application.

### Option 1: Run a command line container on x86 or Arm:

```bash
docker run --rm -it jasonrandrews/ipx-custom-software:base /bin/bash
```

### Option 2: Run a local container with VS Code 

Look [env project](https://github.com/jasonrandrews/env-ipx-custom-software) for more info:

```bash
docker run -d -p 80:80 -p 443:443 jasonrandrews/ipx-custom-software
```

Open http://localhost/?tkn=7a70e21e-01d0-4f01-969c-700919cc35f6 in your browser

### Option 3: Use GitPod

[Open the project in GitPod](https://gitpod.io/#github.com/jasonrandrews/ipx-benchmark-package)

### Option 4: Use GitHub Codespaces

[Open the project in GitHub Codespaces](https://github.com/codespaces)

## Get started
### Step 0:

Get the source code and Arm Fast Model systems to use for developing an application

```bash
git clone https://github.com/jasonrandrews/ipx-benchmark-package.git
```

Everything is located in the ipx-benchmark-package directory.

### Step 0.5:

Before getting started, build the Fast Model systems and example software.

Build the Fast Models systems. All of the systems can be built using the buildall.sh or each individual system can be built using the build.sh in each directory under fastmodels/. 

```bash
cd ipx-benchmark-package/fastmodels
./buildall.sh
cd ..
```

The Fast Model systems which can run software are ready. Next, create a custom benchmark.

### Step 1: 

Follow the instructions in README.txt to compile one of the provided applications or create your own. 

To build the "hello world" application using the GCC ompiler use

```bash
./build_app.sh hello m0px1_nocache GCC
```

To create a new application called `fp-multiply` use:

```bash
cp -r Applications/app-template Applications/fp-multiply
mv Applications/fp-multiply/app-template.c Applications/fp-multiply/fp-multiply.c
```

### Step 2: 

Edit the file `sw_options.json` and set `name`, `description` and `valid_systems` tags.

The `name` tag value should have the name of all applications you would like to upload. For example: `"name": ["fp-multiply"],`

The `valid_systems` tag value should have the system names you want to run your custom software on. For example: `"valid_systems": ["m0px1_nocache"].`

```
{
        "software_name": ["fp-multiply"],
        "software_description":["Floating point multiply test"],
        "valid_compilers": ["AC6","GCC"],
        "valid_systems": ["m0px1_nocache"]
}
```

Save changes in the json file.

To create multiple applications, repeat this step for each application.

### Step 3:

Modify the `Applications/fp-multiply/fp-multiply.c` source file to add your benchmark code between the comments "Enter user defined code here" and "End user defined code here" and save changes. For example, use this `main()` function:

```c
int main()
{
    double a = 3.14;
    double b = 6.023456;
    double product;

    (void) start_marker();

    // Calculating product
    product = a * b;

    (void) stop_marker();

    printf("Product = %.2lf\n", product);

    return 0;
}
```

### Step 4 (optional): 

You can compile your custom software or skip this step in which case your application will be compiled once uploaded.

To compile the application with Arm Compiler for the `fp-multiply` example:

```bash
./build_app.sh fp-multiply m0px1_nocache GCC
```
  
This step creates the compiled software executable in ./Applications/fp-multiply/GCC/fp-multiply.axf


### Step 5 (optional):

To test and debug the application on the Fast Model. Navivate to the system the software was compiled for, Cortex-M0Plus in this case and run.

```bash
cd $HOME/ipx-benchmark-package/fastmodels/Cortex-M0Plus/
./run.sh -a fp-multiply -c GCC
cd ../../
```

### Step 6: 

Run the app checker script.

```bash
./app_checker.py
```

Clean out any Fast Model binaries to make upload quicker, and Tar up your top-level directory and create the upload package.

```bash
cd fastmodels
./cleanall.sh
cd ../..
tar cvzf my-custom-software.tgz ipx-benchmark-package/
```

### Step 7: 

Upload the `my-custom-software.tgz` to Arm IP Explorer and try it using the simulation feature
