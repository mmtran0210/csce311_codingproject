# the compiler to use
CXX = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS = -g -Wall

# the name of the executable file 
TARGET = sloppySim

# the build target executable:
all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp -pthread

clean:
	$(RM) $(TARGET)
