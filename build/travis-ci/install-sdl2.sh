#!/bin/sh
set -e
export SDL_PREFIX=$HOME/sdl2
# check to see if sdl2 folder is empty
if [ ! -d "$SDL_PREFIX" ]; then
  curl -O http://www.libsdl.org/release/SDL2-2.0.3.tar.gz
  tar -xzvf SDL2-2.0.3.tar.gz
  (cd SDL2-2.0.3 && ./configure --prefix=$SDL_PREFIX && make && make install)
else
  echo "Using cached sdl2 directory."
fi
