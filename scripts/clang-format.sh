#!/bin/bash

DIRECTORIES=(
  atomic
)

STYLE="{\
BasedOnStyle: google,\
IndentWidth: 4,\
AccessModifierOffset: -4,\
ColumnLimit: 120\
}"

FORMAT_TMP=format.tmp
touch $FORMAT_TMP

for DIR in ${DIRECTORIES[*]}; do
  for FILE in $(find $DIR -name *.cpp -o -name *.h -o -name *.hpp); do
    clang-format-10 --style="$STYLE" --verbose --Werror $FILE > $FORMAT_TMP
    git --no-pager diff --no-index $FILE $FORMAT_TMP
    cp $FORMAT_TMP $FILE
  done
done

rm $FORMAT_TMP
