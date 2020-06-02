CC = g++
CXXFILES = main.cpp logic.cpp
LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer
TARGET = tetris
PREFIX = /usr/local/bin

all:
	$(CC) $(CXXFILES) $(LIBS) -o $(TARGET)
install:
	install $(TARGET) $(PREFIX)
uninstall:
	rm $(PREFIX)/$(TARGET)