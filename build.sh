#!/bin/bash

git clone git@gitlab:aledr2/zyre-zink.git
cd zyre-zink/zink
cmake -B./build -H. -DCMAKE_BUILD_TYPE=Debug
cd build
make -j`nproc`
