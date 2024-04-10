build: main.cpp
	g++ -o minesweeper main.cpp -l sfml-graphics -l sfml-window -l sfml-system

run: build
	./minesweeper
