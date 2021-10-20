#!/bin/sh

cd /home/FBSRT/
chmod 777 PLCSRT.exe
./PLCSRT.exe 8083 8081 8080 8082 127.0.0.1 -noautostart -cleandb -cleanlog &
