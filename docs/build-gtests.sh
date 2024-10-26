#! /bin/sh

# MIT License
#
# Copyright (c) 2024 Giovanni Santini
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# -------------------------------------------------
#
# You can run this script to build the benchmarks
# with the google benchmark library to compare
# the performance with valfuzz.
# The functions are the same as the ones in the
# valfuzz benchmarks examples.
#
# -------------------------------------------------

GOOGLE_BENCHMARK_DIR=google-benchmark
CPPTEST_DIR=${PWD}
CPPTEST_NAME=gbenchmarks.cpp
OUTPUT_NAME=mybenchmark

g++ $CPPTEST_DIR/$CPPTEST_NAME \
        -std=c++11  \
        -isystem $GOOGLE_BENCHMARK_DIR/include \
        -L$GOOGLE_BENCHMARK_DIR/build/src \
        -lbenchmark -lpthread \
        -o $OUTPUT_NAME
