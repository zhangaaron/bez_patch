CC = g++
CFLAGS = -DGL_GLEXT_PROTOTYPES -Wall -ansi -Ieigen -Iglut-3.7.6-bin
CFLAGS_g = -g -DGL_GLEXT_PROTOTYPES -Wall -ansi -Ieigen -Iglut-3.7.6-bin
LDFLAGS = -lGL -lGLU -lglut
all: main
main: bezier.cpp parser.cpp poly_render.cpp main.cpp 
	$(CC) $(CFLAGS) bezier.cpp parser.cpp poly_render.cpp main.cpp  -o main $(LDFLAGS)

debug: bezier.cpp parser.cpp poly_render.cpp main.cpp 
	$(CC) $(CFLAGS_g) bezier.cpp parser.cpp poly_render.cpp main.cpp  -o main $(LDFLAGS)

clean:
	rm -rf main