#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
compiler="${CXX:-g++}"
"$compiler" -std=c++11 -Wall -Wextra -Werror -pedantic -fsyntax-only -Itests/stub main.cpp ui_model.cpp
printf '%s\n' 'SDL2 native UI: syntax checked with test stub (not linked against SDL-PS4).'
