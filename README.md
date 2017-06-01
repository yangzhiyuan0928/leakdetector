# Leakdetector
A simple C++ memory leak detector in C++11

## Description
The Leakdetector is coded when i read the The Annotated STL Source(using SGI STL), which is a useful Debug tools for dynamic apply memory in C++. Though the Leakdetector only can match new/detete now, I will continue to improve it and make it support:
* new/delete match
* malloc/free match
* overstep visit error
* uninitial variable waring
* r/w released memory

## programming model
* leakdetector switch
* circular bidirection list
* overload operator new/delete

## support
new/delete match

## to do






