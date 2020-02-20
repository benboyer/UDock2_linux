#!/bin/bash

# This is the path using macports, change it according to your local settings
export LIBRARY_PATH="$LIBRARY_PATH:/opt/local/lib"

rm -rf build/*
cd build
cmake ..
make
