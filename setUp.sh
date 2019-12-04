#!/bin/bash
pwd
sudo apt install make cmake g++ libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev -y
pwd
cmake CmakeLists.txt
pwd
make
