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
- `include` – the directory with public headers to include when importing this CMake project
- `src` – source code

## Usage examples

Files in `examples` directory demonstrate how to use this project for depth estimation.

- `GetDepthExample.cpp` accepts three CLI arguments: path to the file with calibration parameters,
  path to the left image and path to the right image – using them, it estimates depth and prints the
  resulting array of floating-point numbers to `stdout`.
