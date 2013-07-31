#!/bin/sh
MAINDIR="$(pwd)"
cd ${MAINDIR}/lib/libevoke
make clean
make
sudo make install
cd ${MAINDIR}/src/nexusd
make clean
make
sudo make install

