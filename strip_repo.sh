#!/usr/bin/env sh

set -e

if [ -f Makefile ]; then
    make clean
fi
rm -rf bin/ test/
rm -f .clang-format Makefile

mv src/* .
rm -rf src/
rm -f strip_repo.sh
