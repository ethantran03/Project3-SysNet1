# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall

# Output executable name
TARGET = lizards

# Source files
SRC = lizards.cpp
# If you completed the extra credit, add lizardsUni.cpp
# SRC = lizards.cpp lizardsUni.cpp

# Object files (we'll create them from the .cpp files)
OBJ = $(SRC:.cpp=.o)

# Build the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Rule to compile .cpp files to .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild everything from scratch
rebuild: clean $(TARGET)

.PHONY: clean rebuild
