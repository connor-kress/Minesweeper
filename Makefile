build: src/main.cpp
	g++ -std=c++11 -o minesweeper src/*.cpp -l sfml-graphics -l sfml-window -l sfml-system

run: build
	./minesweeper
