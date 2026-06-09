# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -Wextra -g

# Files
TARGET  = process_explorer
SRCS    = src/main.cpp src/process.cpp src/process_manager.cpp
OBJS    = $(SRCS:.cpp=.o)

# Default rule — builds the executable
all: $(TARGET)

# Links all object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compiles each .cpp into a .o object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Removes compiled files so you can start fresh
clean:
	rm -f $(OBJS) $(TARGET)
