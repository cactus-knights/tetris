#  Preparation
Install appropriate SDL libraries

## Windows/macOS
Use [download links](https://www.libsdl.org/download-2.0.php) from [SDL website](https://www.libsdl.org)

## Linux
Use command suitable for you

### Debian-based
```bash
$ sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev
```
### Arch-based
```bash
$ sudo pacman -S install sdl2 sdl2_image sdl2_mixer
```

### RPM-based
```bash
$ yum install SDL-devel SDL_mixer-devel SDL_image-devel
```

# Compiling

### Linux
```bash
$ git clone https://github.com/cactus-knights/tetris

$ cd tetris

$ g++ main.cpp logic.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -o tetris
```

## Running

### Linux
```bash
$ ./tetris
```
