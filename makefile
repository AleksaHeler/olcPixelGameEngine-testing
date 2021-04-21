
all:
	g++ -o game main.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

clean:
	rm -f main.o game

# 'vblank_mode=0' is used so we dont sync framerate to monitor refreshrate (unlimited frames)
run: all
	./game
