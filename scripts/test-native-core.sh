#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p .cache
c++ -std=c++11 -Wall -Wextra -Werror -pedantic \
  ps4/src/core/download_queue.cpp ps4/tests/download_queue_test.cpp \
  -o .cache/orion-core-test
.cache/orion-core-test
