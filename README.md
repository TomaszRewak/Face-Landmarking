# FaceLandmarking
Real time face landmarking software.

The approach to the face landmarking problem used in this project is described in detail on my blog: (work in progress). The blog post contains also some information on how I’ve achieved the real time performance.

<p align="center">
  <img src="https://github.com/TomaszRewak/Face-Landmarking/blob/master/About/detection.gif?raw=true" width=400/>
</p>

## About

This software is written (mostly) in C++ (with some ML parts written in Python). For video capturing and processing it uses OpenCV (OpenCV is also used for initial face detection).

In this project, the face landmarking is an iterative process which updates positions of key face points each frame using simple filters and decision trees. At the same time NN autoencoders ensure that the overall shape of the face stays correct.

The algorithm maps 194 points on all of the detected faces each frame. The ML models have been thought using the HELEN dataset: http://www.ifp.illinois.edu/~vuongle2/helen/

It's just a pet project of mine, so still requires some work. In particular I didn't have that much time (nor will :D) to conduct extended experiments. Most of the parameters (like shape and number of filters, size of NN etc.) are just mine educated guesses.

## Setup

This repo contains all the code required to extract features from the dataset, generate ML models and use the face landmarker. Things that are missing are: dependencies (like OpenCV which has to be installed separately) and learning data.

In the main directory of the cloned repo one has to create a `Data` directory with following subdirectories: `annotation`, `images`, `haar`, `mask`, `autoencoder`, `features` and `regressors`.

The dataset can be downloaded from the HELEN project website: http://www.ifp.illinois.edu/~vuongle2/helen/

All of the annotation files (`1.txt` to `2330.txt`) have to be extracted into the `annotation` directory and all of the images (`232194_1.jpg` to `3266693323_1.jpg`) into the `images` dir.

Also, as the software uses pretrained haar filters for initial face detection (at the first frame only), all of the .xml files from https://github.com/opencv/opencv/tree/master/data/haarcascades have to be copied into the previously created `haar` directory.

## Feature extraction

To extract features from the dataset the compiled program has to be ran with a `-type features` flag. This process will create files with learning examples in two directories: `autoencoder` and `features`. Also a `maks/avg-face.mask` file, that contains an average face shape computed based on the entire dataset, will be generated.

Make sure that when you perform this step (or any other of the following steps) the `./../Data` dir is visible to the program. This is the default configuration of the VS project.

The process might take some time.

## Learning process

To generate decision trees and the NN autoencoder, the `autoencoder.py` and the `regressor.py` scripts from the `FaceLandmarking.LearningProcess` dir have to be run. 

This process populates the `regressors` dir.

## Usage

If all of the previous steps finished successfully, the software should be ready to use. 

The best way to test it is to run the program with a `-type example` flag. It will load examples (one by one) from the dataset and display them. On each spacebar hit the program will perform one step of mask adjustment. Any other key stroke will change the image to the next one.

Finally, a `-type video` flag can be used to load a video for a file (provided with a `-video [path]` flag). Unfortunately I didn't have time to play with video settings, so the program doesn't read information about the orientation of the video. It (as well as the size of the video) might have to be adjusted manually (using `-transform`, `-transform-rotate`, `-transform-width` and `-transform-height` flags - see `main.cpp` for their default values). To initialize face landmarking process simply hit space – it will run haar filters on given frame to detect the initial position of the face.

## More

If you have any questions fell free to reach out to me.
