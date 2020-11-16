#!/bin/bash

cd /home
ls -al
./build/ZPR-2020Z-atomic-list-test

gcovr -f ".*atomic-list.*"  -r .
