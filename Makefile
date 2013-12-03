CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -Weffc++ -g
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf

TARGET = jumpman
OBJS = src/Sprite.o src/Player.o src/BasicStar.o src/MovingStar.o\
         src/GraphicsEngine.o src/Game.o src/main.o

$(TARGET): $(OBJS)
	        $(CXX) $^ $(LDFLAGS) -o $(TARGET)

clean:
	        $(RM) $(OBJS) $(TARGET)
