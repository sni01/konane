konane: Board.o Player.o main.o
	g++ Board.o Player.o main.o -o konane

Board.o: Board.h Board.cpp
	g++ -c Board.cpp

Player.o: Player.h Board.h Player.cpp
	g++ -c Player.cpp

main.o: Board.h Player.h main.cpp
	g++ -c main.cpp

clean:
	rm *.o *.pyc konane
