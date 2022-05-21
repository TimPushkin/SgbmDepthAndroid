# SgbmAndroid

Android library for depth estimation based on
[StereoSGBM](https://docs.opencv.org/4.5.5/d2/d85/classcv_1_1StereoSGBM.html) from OpenCV.

The library consists of a single class called `DepthEstimator` which computes depth maps from an
image pair (the so-called "stereo pair") based on the calibration parameters of the cameras which
took the images.

<img src="https://i.imgur.com/JGXdks2.png" alt="Depth map example"/>

The current implementation is capable to compute up to 10 depth maps per second on modern Android
smartphones.

<img src="https://i.imgur.com/MNK3o9C.png" alt="Performance plot"/>

## Getting started

To build this project you need an OpenCV build suitable for the required Android ABI in your system.
Two options are available:

1. **(Recommended)** Run `BuildOpenCV.cmake` script which will download and build OpenCV
   automatically with the recommended optimizations. Read more about how to run the script in the
   [native part's README](sgbmandroid/src/main/cpp/README.md). You have to run it once for every
   Android ABI you plan to use.
2. Download and install the prebuilt [OpenCV Android SDK](https://opencv.org/releases). It may lack
   some possible optimizations.

After OpenCV is installed build the project with Gradle -- the library is available in
`:sgbmandroid` module. It builds for all ABIs.

Currently, the library is automatically generated from its native implementation, so no
documentation is available in the resulting code. To read the `DepthEstimator` documentation see
[its native header](sgbmandroid/src/main/cpp/include/DepthEstimator.h).

## Demo application

An example application is available in `:app` module. It computes a depth map from a pair of images
using the chosen calibration parameters and displays the resulting image also showing the time it
took to compute. You can get a sample stereo pair and calibration parameters for it
[here](sgbmandroid/src/main/cpp/examples/images) to try it out and test the performance.

<img src="https://i.imgur.com/Ch1MW4G.png" alt="Demo application GUI"/>
