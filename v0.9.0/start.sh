#!/bin/sh

cd FBSRT/
chmod 777 PLCSRT.exe
chmod +777 "/dev/ttyS0"
chmod +777 "/dev/ttyUSB0"
chmod +777 "/dev/gpiomem"
./PLCSRT.exe 8083 8081 8080 8082 127.0.0.1 &
