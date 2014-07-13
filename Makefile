CC = gcc
CFLAGS = -Wall `sdl-config --cflags`
LIBS = `sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_mixer
NAME = 'coinage'
HEADERS = src/box.h src/list.h src/qtree.h src/item.h src/timer.h src/player.h src/floor.h src/graphics.h src/ui_init.h src/ui.h
SOURCES = src/main.c $(HEADERS:.h=.c)
OBJECTS = $(SOURCES:.c=.o)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

box.o: box.h
list.o: list.h
qtree.o: qtree.h list.o box.o
timer.o: timer.h

item.o: item.h
floor.o: floor.h item.o timer.o qtree.o
player.o: player.h floor.o

ui_init.o: ui_init.h
graphics.o: graphics.h ui.h
ui.o: ui.h ui_init.o graphics.o player.o timer.o
main.o: $(HEADERS) ui.o


clean:
	rm -f *.o
	cp $(NAME) ../$(NAME)
	rm $(NAME)
