#makefile to compile player.cpp in player directory into player.o
# compile server.cpp in server directory into GS.o


CC = g++
#CFLAGS = -Wall -g -std=c++11
#LDFLAGS = -lncurses -lpthread

all: clean 
	$(CC) playerDir/player.cpp -std=c++17 -o player
	$(CC) server/server.cpp -std=c++17 -o GS

clean:
	rm -f *.o *.out

squeaky: clean
	rm -r server/GAMES || true
	rm -r server/SCORES || true
	$(CC) playerDir/player.cpp -std=c++17 -o player
	$(CC) server/server.cpp -std=c++17 -o GS