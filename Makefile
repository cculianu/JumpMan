CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -effc++ -g
LDFLAGS = -lSDL -lSDL_image

TARGET = roborally
OBJS = src/Sprite.o src/Tile.o src/Game.o src/main.o

$(TARGET): $(OBJS)
	        $(CXX) $^ $(LDFLAGS) -o $(TARGET)

clean:
	        $(RM) $(OBJS) $(TARGET)
