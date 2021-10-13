#!/bin/sh
MYDIR=`dirname $0`
sudo cp -r ${MYDIR}/include/. /usr/include
sudo cp -r ${MYDIR}/lib/. /usr/lib
echo "Environment has been set up from ${MYDIR}."