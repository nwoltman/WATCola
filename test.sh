#!/bin/bash

echo "Script started on `date`"

echo ""

echo "TEST 1 (no input) RUN:"
echo "./soda"
echo "INFINITE LOOP"
echo "TEST 1 COMPLETE."

echo ""

echo "TEST 2 (explicit config) RUN:"
echo "./soda soda.config"
echo "INFINITE LOOP"
echo "TEST 2 COMPLETE."

echo ""

echo "TEST 3 (explicit config and seed) RUN:"
echo "./soda soda.config 100"
echo "INFINITE LOOP"
echo "TEST 3 COMPLETE."

echo ""

echo "TEST 4 (bad config file) RUN:"
echo "./soda xxx"
./soda xxx
echo "TEST 4 COMPLETE."

echo ""

echo "TEST 5 (bad input seed) RUN:"
echo "./soda soda.config a"
./soda soda.config a
echo "TEST 5 COMPLETE."

echo ""

echo "Script ended on `date`"
