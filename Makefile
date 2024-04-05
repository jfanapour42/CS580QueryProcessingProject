mainmake: relation.cpp main.cpp
	g++ -std=c++17 -o project relation.cpp main.cpp
clean:
	-rm project