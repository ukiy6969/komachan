C_PLUSPLUS=g++ -std=c++11
game: board.o search.o io.o game.o main.o
	$(C_PLUSPLUS) -Wall -o game main.o board.o search.o io.o game.o -O4
board.o: board.cc board.h
	$(C_PLUSPLUS) -c board.cc -O4
search.o: search.cc search.h
	$(C_PLUSPLUS) -c search.cc -O4
game.o: game.cc game.h
	$(C_PLUSPLUS) -c game.cc -O4
io.o: io.cc io.h
	$(C_PLUSPLUS) -c io.cc -O4
main.o: main.cc
	$(C_PLUSPLUS) -c main.cc -O4
