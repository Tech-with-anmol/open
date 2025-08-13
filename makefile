CC = g++
CFLAGS = -Iinclude
LDFLAGS = -lglfw -ldl -lGL

SRC = openmf.cpp src/glad.c
OBJ = $(SRC:.cpp=.o)

myapp: $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

