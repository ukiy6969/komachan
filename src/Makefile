C_PLUSPLUS=g++ -std=c++11
game: board.o search.o io.o game.o main.o
	$(C_PLUSPLUS) -Wall -o game main.o board.o search.o io.o game.o -O3
board.o: board.cc board.h
	$(C_PLUSPLUS) -c board.cc -O3
search.o: search.cc search.h
	$(C_PLUSPLUS) -c search.cc -O3
game.o: game.cc game.h
	$(C_PLUSPLUS) -c game.cc -O3
io.o: io.cc io.h
	$(C_PLUSPLUS) -c io.cc -O3
main.o: main.cc
	$(C_PLUSPLUS) -c main.cc -O3
