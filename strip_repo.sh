#!/usr/bin/env sh

set -e

make clean
rm -rf bin/ test/
rm -f .clang-format Makefile strip_repo.sh
