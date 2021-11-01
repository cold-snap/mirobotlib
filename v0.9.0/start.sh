#!/bin/sh

cd /home/ubuntu/runtime/v0.9.0
chmod 777 PLCSRT.exe
sudo chmod +777 "/dev/ttyS0"
sudo chmod +777 /dev/gpiomem
./PLCSRT.exe 8083 8081 8080 8082 127.0.0.1
