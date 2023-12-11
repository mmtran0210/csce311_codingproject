# Define the compiler
CXX = g++

# Define compiler flags
CXXFLAGS = -Wall -pthread

# Define the name of the executable
TARGET = mapper

# Define source files
SOURCES = main.cpp HashTable.cpp OpQueue.cpp

# Define object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link the target with object files
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean target
clean:
	rm -f $(TARGET) $(OBJECTS) $(INPUT) $(OUTPUT)

# Run target
run:
	./$(TARGET) basicTests*.txt basicTest_OUTPUT_*.txt

.PHONY: all clean run
