#!/bin/bash

DIRECTORIES=(
  atomic-list
)

FORMAT_TMP=format.tmp

for DIR in ${DIRECTORIES[*]}; do
  for FILE in $(find $DIR -name *.cpp -o -name *.h); do
    clang-format-10 --style=Google --verbose --Werror $FILE > $FORMAT_TMP
    git diff --no-index $FILE $FORMAT_TMP
    cp $FORMAT_TMP $FILE
  done
done

rm $FORMAT_TMP
