CFLAGS = -Wall
LDFLAGS = -lSDL2 -lSDL2_gfx -lm

objects = wireframe.o perspect.o objread.o commline.o

all: wireframe

wireframe: $(objects)
	g++ $(objects) $(CFLAGS) -o wireframe $(LDFLAGS)

wireframe.o: wireframe.c perspect.o objread.o commline.o
	g++ $(CFLAGS) objread.c perspect.c commline.c datastruct.h -c wireframe.c

perspect.o: perspect.c objread.c commline.c
	gcc $(CFLAGS) -c perspect.c

objread.o: objread.c commline.c commline.h
	gcc $(CFLAGS) -c objread.c

commline.o: commline.c
	gcc $(CFLAGS) -c commline.c

clean:
	rm -f *.o *~

purge: clean
	rm -f wireframe