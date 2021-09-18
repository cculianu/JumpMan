#!/bin/bash

set -v

em++ -Os -std=c++17 -s ALLOW_MEMORY_GROWTH=1  -s USE_SDL=2 -s WASM=1 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS='["png"]' -s EXIT_RUNTIME=1 -lidbfs.js --emrun --preload-file graphics --preload-file audio --shell-file shell_minimal.html  -o index.html src/*.cpp
