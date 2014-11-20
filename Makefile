CC = g++
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -Wall -ansi -Ieigen -Iglut-3.7.6-bin -o1
LDFLAGS = -lGL -lGLU -lglut
all: main
main: bezier.cpp parser.cpp poly_render.cpp main.cpp 
	$(CC) $(CFLAGS) bezier.cpp parser.cpp poly_render.cpp main.cpp  -o main $(LDFLAGS)

clean:
	rm -rf main