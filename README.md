# FaceLandmarking
Face landmarking software for lips movement detection.

![Detection](https://github.com/TomaszRewak/Face-Landmarking/blob/master/About/detection.gif?raw=true)

This software is written (mostly) in C++ (with ML parts written in Python) using OpenCV (for video capture and processing). The algorithm itself is written from a scratch. It's based on decision trees and active mask that have been taught using the Helen dataset.

This project contains all modules needed to extract features from the dataset, teach the model and use the so prepared mask for face landmarking.

Right now it lacks some resources (needed to run it) and needs some cleaning, but I simply don't want to lose all this work ;) I will describe everything in more details in the future. 
