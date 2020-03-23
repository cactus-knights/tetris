# Tetris

The SDL version of classic Tetris game.

[Скачать](https://github.com/cactus-knights/tetris/blob/master/2.md)
## Prerequisites

Install appropriate SDL libraries

### Linux
#### Debian-based
```bash
$ sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev
```
#### RPM-based
```bash
$ yum install SDL-devel SDL_mixer-devel SDL_image-devel
```
#### Arch/Manjaro
```bash
$ sudo pacman -S install sdl2 sdl2_image sdl2_mixer
```
### Windows/Mac OS X
Use [download links](https://www.libsdl.org/download-2.0.php) from [SDL website](https://www.libsdl.org/index.php)

## Compiling

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

## Authors

* **Aleksey Chensov** - *Coding* - [1007Leo](https://github.com/1007Leo)
* **Andrey Gorshkov** - *Coding* - [sticky-tea](https://github.com/sticky-tea)
* **Mikhail Levin** - *Design* - [mephistolie](https://github.com/mephistolie)
* **Mikhail Bobilev** - *Ideological lider*

## Acknowledgments
* Andrey Melekhov
* Aleksandr Abanin
