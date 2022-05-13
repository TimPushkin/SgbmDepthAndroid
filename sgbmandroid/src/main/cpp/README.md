# Native core of SgbmAndroid library

This directory contains the native implementation of the SGBM-based depth estimation algorithm.

## Description

[StereoSGBM](https://docs.opencv.org/4.5.5/d2/d85/classcv_1_1StereoSGBM.html) from OpenCV is used to
calculate depth given a pair of images and calibration parameters. The result is an array of
floating-point numbers representing depth of each pixel of the estimated depth map.

**Calibration parameters** should be stored in a format accepted by
[FileStorage](https://docs.opencv.org/4.5.5/da/d56/classcv_1_1FileStorage.html) from OpenCV. The
parameters are:

1. Maps of `x` and `y` values needed to prepare each image of the image pair (4 maps in total) for
   depth estimation with OpenCV's
   [remap](https://docs.opencv.org/4.5.5/da/d54/group__imgproc__transform.html#gab75ef31ce5cdfb5c44b6da5f3b908ea4)
   method.
2. A perspective transformation matrix `Q` used to create a 3D representation of image disparity
   with OpenCV's
   [reprojectImageTo3D](https://docs.opencv.org/4.5.5/d9/d0c/group__calib3d.html#ga1bc1152bd57d63bc524204f21fde6e02)
   method.

**Image pair** is a pair of left and right photos of something taken roughly at the same moment (if
the scene being shot is dynamic), from the same perspective and with some distance between them (
also known as a *baseline*). Images should be compatible with OpenCV's
[imdecode](https://docs.opencv.org/4.5.5/d4/da8/group__imgcodecs.html#ga26a67788faa58ade337f8d28ba0eb19e)
method.

## Project structure

The project consists of the following directories:

- `examples` – usage examples (see below)
- `external` - 3rd-party libraries
- `include` – public headers to include when importing this CMake project
- `src` – source code

## Build

This project requires [OpenCV](https://github.com/opencv/opencv) 4.5.5 or later. You can either
manually download and install a version of OpenCV you would like to be used in the build, or run
`BuildOpenCV.cmake` script that will download and build OpenCV for you.

If you already have OpenCV, you can build this project with CMake:

```shell
cmake .          # Configure the build
cmake --build .  # Start the build
```

### Running `BuildOpenCV.cmake`

The script will download the required version of OpenCV and build it with the optimal configuration.
OpenCV will reside in `build/opencv`, where this project will search for it first during its build.

The script can be run as follows:

```shell
cmake <options> -P BuildOpenCV.cmake
```

If the project is going to be built and used on the same platform where the script is run, no
options are required as they are going to be inferred from the system by CMake. Alternatively, if
you plan to build it for some other platform, you can specify the following options in the form
of `-D<variable>=<value>`:

- `CMAKE_GENERATOR` -- a generator to use (for example, `Ninja`)
- `CMAKE_TOOLCHAIN_FILE` -- path to a CMake toolchain file
- `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` -- C/C++ compiler to use (for example, `clang`
  /`clang++` or a path to a compiler)
- `ANDROID_ABI` -- if building for Android, for which ABI to build (should be one of
  the [supported ABIs](https://developer.android.com/ndk/guides/abis))
- `ANDROID_ARM_NEON` -- if building for Android, whether to let OpenCV make use of Neon or not (`ON`
  or `OFF`)

The toolchain file specified can influence the other variables default values.

For example, if you want to build for Android with ABI `armeabi-v7a` and Neon support, and you have
NDK installed in the system, the command to run the script will be something like:

```shell
cmake -DCMAKE_GENERATOR=Ninja -DCMAKE_TOOLCHAIN_FILE="<NDK path>/build/cmake/android.toolchain.cmake" -DCMAKE_C_COMPILER="<NDK path>/toolchains/llvm/prebuilt/<platform>/bin/clang.exe" -DCMAKE_CXX_COMPILER="<NDK path>/toolchains/llvm/prebuilt/<platform>/bin/clang++.exe" -DANDROID_ABI=armeabi-v7a -DANDROID_ARM_NEON=ON -P BuildOpenCV.cmake
```

### Generating a language interface

An interface for languages like Java, Objective-C, Python, and some other can be automatically
generated for this project. [Scapix](https://github.com/scapix-com/scapix) is used for this --
see its page for the set of the supported languages.

To generate an interface modify the configure command as follows:

```shell
cmake -DSCAPIX_BRIDGE=<language> .
```

The interface will be placed in the corresponding subdirectory of `generated` directory after the build.

## Usage examples

Files in `examples` directory demonstrate how to use this project for depth estimation.

- `GetDepthExample.cpp` accepts three CLI arguments: path to the file with calibration parameters,
  path to the left image and path to the right image – using them, it estimates depth and prints the
  resulting array of floating-point numbers to `stdout`.
