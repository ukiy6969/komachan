komachan: board.o data.o ini.o io.o search.o
	gcc -o bin/komachan src/board.o src/data.o src/ini.o src/io.o src/search.o src/main.c

board.o: src/board.c
	gcc -c src/board.c -o src/board.o

data.o: src/data.c
	gcc -c src/data.c -o src/data.o

ini.o: src/ini.c
	gcc -c src/ini.c -o src/ini.o

io.o: src/io.c
	gcc -c src/io.c -o src/io.o

search.o: src/search.c
	gcc -c src/search.c -o src/search.o

