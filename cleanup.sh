#!/usr/bin/env sh

set -xe

if [ -f Makefile ]; then
    make clean
fi
rm -rf bin/ test/
rm -f .clang-format .gitignore Makefile

mv src/* .
rmdir src/
rm -f cleanup.sh
