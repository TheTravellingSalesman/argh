# target executable name
TARGET = Sim04

# Compiler
CC=g++

# Compiler Flags
CXXFLAGS = -g -Wall -std=c++11

# Source files
SOURCES = Config.cpp Timer.cpp ProcessControlBlock.cpp OperatingSystem.cpp Log.cpp Lock.cpp ResourceManager.cpp Sim04.cpp

# header file dependencies
HEADERS = Config.h Timer.h ProcessControlBlock.h OperatingSystem.h Log.h Lock.h ResourceManager.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

#default target
all: $(TARGET)

# link everything together
$(TARGET):	$(OBJECTS)
				$(CC) -pthread $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS):	$(SOURCES) $(HEADERS)
				$(CC) $(CXXFLAGS) -c $(SOURCES)

# Clean target
clean:
	find . -type f | xargs touch
	rm -rf $(TARGET) $(OBJECTS)