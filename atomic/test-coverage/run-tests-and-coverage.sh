#!/bin/sh

cd /home
./build/ZPR-2020Z-atomic-list-test

gcovr -f "atomic/src/.*" -r .
