#!/bin/bash

TEST_PROJECT=ZPR-2020Z-atomic-list-test
TEST=$(find . -name $TEST_PROJECT)

echo "Running tests for $TEST_PROJECT:"
echo ""
$TEST

echo ""
echo "Running tests coverage for $TEST_PROJECT:"
echo ""

gcovr -f "atomic/src/.*" -r .
