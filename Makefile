# Compiler and flags
CXX := g++
CXXFLAGS := -IAOCHelper -Wall -Wextra -std=c++20

# BUILD DIR
BUILD_DIR := build

# AOCHelper source and object
HELPER_SRC := AOCHelper/AOCHelper.cpp
HELPER_OBJ := $(BUILD_DIR)/AOCHelper.o

# Directories for the days
DAYS := $(shell seq 1 31 | sed 's/^/Day/')

# Object and executable rules
OBJS := $(DAYS:%=$(BUILD_DIR)/%.o)
EXES := $(DAYS:%=%/solution.exe)

# Default target
.PHONY: all
all: $(EXES)

$(DAYS): % : %/solution.exe

# Rule to build individual executables
%/solution.exe: $(BUILD_DIR)/%.o $(HELPER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to build object files for each day's source
$(BUILD_DIR)/%.o: %/solution.cpp AOCHelper/AOCHelper.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build AOCHelper object
$(HELPER_OBJ): $(HELPER_SRC) AOCHelper/AOCHelper.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(EXES)

.PHONY: clean

