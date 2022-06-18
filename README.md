# Setup

### First time?
```
mkdir build
cd build
cmake ..
```

#### Install SDL2 Linux
```
apt-cache search libsdl2
sudo apt-get install libsdl2-dev
```
You should be able to find `SDL.h` in `/usr/include/SDL2`

#### Install SDL2 OSX
```
brew install sdl2
```
Apple silicon 
```
echo "export PATH=/opt/homebrew/bin:$PATH" >> ~/.zshrc && source ~/.zshrc
```

### Build it
```
cd build
cmake --build .
```

### Run it
```
cd build
./Chip8 <ROM Path>
```
Example: `./Chip8 ../roms/TETRIS`

# References
SDL2: http://lazyfoo.net/tutorials/SDL/index.php

SDL2 API: https://wiki.libsdl.org/CategoryAPI / https://wiki.libsdl.org/APIByCategory

https://en.wikipedia.org/wiki/CHIP-8

https://austinmorlan.com/posts/chip8_emulator/

https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/

Cowgod's Chip-8 Technical Reference v1.0: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
