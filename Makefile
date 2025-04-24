# Set the C++ compiler
CXX = g++

# Set the C++ flags for compiling
CXXFLAGS = -std=c++11 `pkg-config --cflags opencv4`

# Set the linking flags for OpenCV
LDFLAGS = `pkg-config --libs opencv4`

# Define the output binary name
TARGET = app

# Define the source file(s)
SRCS = main.cpp merge.cpp timestamp.cpp

# Define the object file(s)
OBJS = $(SRCS:.cpp=.o)

# The default target to build when running `make`
all: $(TARGET)

# Rule to create the executable from object files
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to create object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up generated files (e.g., .o and executable)
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets to prevent conflicts with files named 'clean' or 'all'
.PHONY: all clean
