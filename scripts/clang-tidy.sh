#!/bin/bash

DIRECTORIES=(
  atomic
)

BUILD_DIR=$(find . -name Makefile | grep -oP "\./[^/]+/Makefile" | grep -oP "\./\K[^/]+")

for DIR in ${DIRECTORIES[*]}; do
  for FILE in $(find $DIR -name *.cpp -o -name *.h); do
    echo "Running clang-tidy for $FILE"
    clang-tidy-10 -p $BUILD_DIR $FILE -checks=cppcoreguidelines-*,-cppcoreguidelines-owning-memory,-cppcoreguidelines-special-member-functions,misc-*
  done
done
