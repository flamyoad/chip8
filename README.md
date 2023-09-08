![ezgif-2-7d124cadb7](https://user-images.githubusercontent.com/35066207/174448574-5261839d-77ae-49b1-8c2f-3c35eb8a19c1.gif)

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

### Install SDL2 Windows
```
msys2
pacman -S mingw-w64-x86_64-SDL2
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
