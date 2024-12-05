# Compiler and flags
CXX := g++
CXXFLAGS := -IAOCHelper -std=c++20 -Wall -O2

# Build directory
BUILD_DIR := build

# AOCHelper files
HELPER_SRC := AOCHelper/AOCHelper.cpp
HELPER_OBJ := $(BUILD_DIR)/AOCHelper.o

# Default target
all:
	@echo "Specify a day to build, e.g., make day1"

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build AOCHelper object file
$(HELPER_OBJ): $(HELPER_SRC) | $(BUILD_DIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Rules for each day's solution object file
$(BUILD_DIR)/Day%.o: Day%/solution.cpp | $(BUILD_DIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Build solution for a specific day
day%: $(BUILD_DIR)/Day%.o $(HELPER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o Day$*/solution.exe
	@echo "Built Day$* solution. Run it using ./Day$*/solution.exe"

# Clean all build artifacts
clean:
	rm -rf $(BUILD_DIR) Day*/solution.exe

.PHONY: clean all
