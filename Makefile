CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -g
LDFLAGS = -lncursesw

TARGET = roborally
OBJS = src/main.o

$(TARGET): $(OBJS)
	        $(CXX) $^ $(LDFLAGS) -o $(TARGET)

clean:
	        $(RM) $(OBJS) $(TARGET)
