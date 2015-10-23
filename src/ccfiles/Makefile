C_PLUSPLUS=g++ -std=c++11
game: board.o search.o io.o game.o main.o
	$(C_PLUSPLUS) -Wall -o game main.o board.o search.o io.o game.o
board.o: board.cc
	$(C_PLUSPLUS) -c board.cc
search.o: search.cc
	$(C_PLUSPLUS) -c search.cc
game.o: game.cc
	$(C_PLUSPLUS) -c game.cc
io.o: io.cc
	$(C_PLUSPLUS) -c io.cc
main.o: main.cc
	$(C_PLUSPLUS) -c main.cc
