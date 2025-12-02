##
# Project Title
#
# @file
# @version 0.1

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++23 -g -O1 -fsanitize=address -fsanitize=undefined -Isrc/Headers
LDFLAGS := -fsanitize=address -fsanitize=undefined

SRC_DIR := src
BUILD_DIR := build
HEADER_DIR := src/Headers

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := tq

all: $(TARGET)

# linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ -lncursesw $(LDFLAGS)

# compiling
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
