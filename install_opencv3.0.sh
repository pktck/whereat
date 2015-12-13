#!/bin/bash

pause() {
    echo;
    read -n1 -r -p "Press any key to continue..." key;
    echo;
}


#sudo apt-get -y install build-essential cmake git pkg-config \
    #libjpeg8-dev libtiff4-dev libjasper-dev libpng12-dev \
    #libgtk2.0-dev \
    #libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    #libatlas-base-dev gfortran \
    #python2.7-dev

#pause

cd ~
#wget https://github.com/Itseez/opencv/archive/3.0.0.zip
#unzip opencv-3.0.0.zip
cd opencv-3.0.0
#mkdir build
cd build

#pause

#cmake -D CMAKE_BUILD_TYPE=RELEASE \
    #-D CMAKE_INSTALL_PREFIX=/usr/local \
    #-D INSTALL_C_EXAMPLES=ON \
    #-D INSTALL_PYTHON_EXAMPLES=ON \
    #-D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules \
    #-D BUILD_EXAMPLES=ON ..

#pause

make -j4

pause

sudo make install

pause

sudo ldconfig


