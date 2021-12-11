CC = gcc
CFLAGS = -g -std=c++11 -lstdc++
all : compfs

compfs: codeGen.hpp codeGen.cpp staticSemantics.hpp staticSemantics.cpp parser.hpp parser.cpp scanner.hpp scanner.cpp main.cpp
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm compfs *.asm
