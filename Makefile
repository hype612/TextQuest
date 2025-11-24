##
# Project Title
#
# @file
# @version 0.1

CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++23 -g -O1 -fsanitize=address,undefined -Isrc/Headers
LDFLAGS := -fsanitize=address,undefined

SRC_DIR := src
BUILD_DIR := build
HEADER_DIR := src/Headers

SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET := textquest

all: $(TARGET)

# linking
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ -lncurses $(LDFLAGS)

# compiling
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
