CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -Weffc++ -g
LDFLAGS = -lSDL -lSDL_image -lSDL_ttf -l SDL_mixer

ifeq ($(mode),release)
  CXXFLAGS += -O2
endif

ifeq ($(compiler),clang)
  CXX = clang++
endif

TARGET = jumpman
OBJS = src/Sprite.o src/Player.o src/BasicStar.o src/MovingStar.o\
       src/GraphicsEngine.o src/AudioEngine.o\
       src/Highscore.o src/Game.o src/main.o

$(TARGET): $(OBJS)
	        $(CXX) $^ $(LDFLAGS) -o $(TARGET)

clean:
	        $(RM) $(OBJS) $(TARGET)
