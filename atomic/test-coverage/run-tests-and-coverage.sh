#!/bin/sh

cd /home
./build/ZPR-2020Z-atomic-list-test

gcovr -r .
