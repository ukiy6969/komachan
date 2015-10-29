C_PLUSPLUS=g++ -std=c++11
game: board.o search.o io.o game.o main.o
	$(C_PLUSPLUS) -Wall -o game main.o board.o search.o io.o game.o
board.o: board.cc board.h
	$(C_PLUSPLUS) -c board.cc
search.o: search.cc search.h
	$(C_PLUSPLUS) -c search.cc
game.o: game.cc game.h
	$(C_PLUSPLUS) -c game.cc
io.o: io.cc io.h
	$(C_PLUSPLUS) -c io.cc
main.o: main.cc
	$(C_PLUSPLUS) -c main.cc
