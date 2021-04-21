
all:
	g++ -o game main.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

clean:
	rm -f main.o game

run: all
	./game
