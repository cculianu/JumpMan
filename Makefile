CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -Weffc++ -g -O2
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -l SDL_mixer

TARGET = jumpman
OBJS = $(addsuffix .o, $(basename $(wildcard src/*.cc)))

ifeq ($(compiler),clang)
  CXX = clang++
endif

$(TARGET): $(OBJS)
	        $(CXX) $^ $(LDFLAGS) -o $(TARGET)

clean:
	        $(RM) $(OBJS) $(TARGET)
