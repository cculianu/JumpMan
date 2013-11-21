CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -g
LDFLAGS = -lSDL -lSDL_image

TARGET = roborally
OBJS = src/Game.o src/main.o

$(TARGET): $(OBJS)
	        $(CXX) $^ $(LDFLAGS) -o $(TARGET)

clean:
	        $(RM) $(OBJS) $(TARGET)
