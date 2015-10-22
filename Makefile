komachan: board.o io.o data.o search.o ini.o mt.o
	gcc -Wall -O2 src/main.c src/board.o src/io.o src/data.o src/search.o src/ini.o src/lib/mt.o -o komachan
board.o: src/board.c
	gcc -c src/board.c -o src/board.o
io.o: src/io.c
	gcc -c src/io.c -o src/io.o
data.o: src/data.c
	gcc -c src/data.c -o src/data.o
search.o: src/search.c
	gcc -c src/search.c -o src/search.o
ini.o: src/ini.c
	gcc -c src/ini.c -o src/ini.o
mt.o: src/lib/mt19937-64.c
	gcc -c src/lib/mt19937-64.c -o src/lib/mt.o
