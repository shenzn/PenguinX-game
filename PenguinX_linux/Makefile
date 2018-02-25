TARGET=penguinx
LIBS = `sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_gfx -lSDL_mixer
CFLAGS = `sdl-config --cflags`
CC = g++ -O3 

all:
	$(CC) $(CFLAGS) -c game.cpp -o penguinx.o
	$(CC) -o $(TARGET) penguinx.o $(LIBS)
	strip $(TARGET)
clean:
	rm -f ./*.o $(TARGET) core
