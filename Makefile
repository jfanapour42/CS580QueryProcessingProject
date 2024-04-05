CC = g++
CFLAGS = -std=c++17 -Wall -Werror
Target = main.cpp
SOURCES = relation.cpp

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

g++ -std=c++17 -o project relation.cpp main.cpp