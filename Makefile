CXX := g++
NOTCURSES_CFLAGS := $(shell pkg-config --cflags notcurses-core)
NOTCURSES_LIBS := $(shell pkg-config --libs notcurses-core)
# Shared flags (no sanitizers here)
BASE_CXXFLAGS := -Wall -Wextra -std=c++23 -g -Isrc/Headers $(NOTCURSES_CFLAGS)
SAN_FLAGS := -fsanitize=address -fsanitize=undefined
CXXFLAGS := $(BASE_CXXFLAGS) -O1 $(SAN_FLAGS)
LDFLAGS := $(SAN_FLAGS)
LDLIBS := $(NOTCURSES_LIBS) -lncursesw
SRC_DIR := src
BUILD_DIR := build
BUILD_DIR_DEBUG := build-debug
HEADER_DIR := src/Headers
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR_DEBUG)/%.o,$(SRCS))
TARGET := tq
TARGET_DEBUG := tq-debug

all: $(TARGET)

# plain build (default, no sanitizers)
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(NOTCURSES_LIBS) -lncursesw
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(BASE_CXXFLAGS) -O2 -c $< -o $@

# sanitized build, for isolating sanitizer-only issues
debug: $(TARGET_DEBUG)
$(TARGET_DEBUG): $(DEBUG_OBJS)
	$(CXX) $(DEBUG_OBJS) -o $@ $(LDFLAGS) $(LDLIBS)
$(BUILD_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR_DEBUG) $(TARGET) $(TARGET_DEBUG)
.PHONY: all clean debug
