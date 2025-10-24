##
# Project Title
#
# @file
# @version 0.1

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++21 -Isrc/Headers

SRD_DIR := src
BUILD_DIR := build
HEADER_DIR := src/Headers

SRCS := $(shell find $(SRD_DIR) -name '*.cpp')

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := textquest
all: $(TARGET)

# linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ -lncurses

# Compiling
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean


# end
