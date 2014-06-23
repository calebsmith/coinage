CC = gcc

CFLAGS = -Wall `sdl-config --cflags`

NAME = 'chip'

SOURCES = src/*.c

LIBS = `sdl-config --libs` -lSDL_image -lSDL_ttf


$(NAME): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(NAME)

clean:
	rm -f *.o
	cp $(NAME) ../$(NAME)
	rm $(NAME)
