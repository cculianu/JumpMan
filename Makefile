CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -Weffc++ -g -O2 -I/opt/local/include
LDFLAGS = -L/opt/local/lib -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -lSDLmain -framework CoreFoundation -framework Cocoa

TARGET = jumpman
OBJS = $(addsuffix .o, $(basename $(wildcard src/*.cpp)))

ifeq ($(compiler),clang)
  CXX = clang++
endif

$(TARGET): $(OBJS)
	        $(CXX) $^ $(LDFLAGS) -o $(TARGET)

clean:
	        $(RM) $(OBJS) $(TARGET)
