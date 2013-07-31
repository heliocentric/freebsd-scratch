#!/bin/sh
MAINDIR="$(pwd)"
cd ${MAINDIR}/lib/libevoke
make clean
cd ${MAINDIR}/src/nexusd
make clean
