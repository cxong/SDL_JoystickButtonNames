#!/bin/sh
set -e
curl -O http://www.libsdl.org/release/SDL2-2.0.4.tar.gz
tar -xzvf SDL2-2.0.4.tar.gz
(cd SDL2-2.0.4 && ./configure && make && sudo make install)
