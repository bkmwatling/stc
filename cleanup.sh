#!/usr/bin/env sh

set -xe

[ -f Makefile ] && make clean
rm -rf bin/ test/
rm -f .clang-format Makefile

mv src/* .
rmdir src/
rm -f cleanup.sh
