#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
compiler="${CXX:-g++}"
out="$(mktemp -t orion-ui-model-XXXXXX)"
trap 'rm -f "$out"' EXIT
"$compiler" -std=c++11 -Wall -Wextra -Werror -pedantic ui_model.cpp ui_model_test.cpp -o "$out"
"$out"
