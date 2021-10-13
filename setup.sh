#!/bin/sh
sudo apt-get update
sudo apt-get install  libmodbus-dev
sudo apt-get install  wiringpi

MYDIR=`dirname $0`
sudo cp -r ${MYDIR}/include/. /usr/include
sudo cp -r ${MYDIR}/lib/. /usr/lib

sudo ldconfig

echo "Environment has been set up from ${MYDIR}."