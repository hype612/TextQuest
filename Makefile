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
HEADER_DIR := src/Headers
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
PLAIN_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)-plain/%.o,$(SRCS))
TARGET := tq
TARGET_PLAIN := tq-plain

all: $(TARGET)

# sanitized build (default, existing behavior)
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# plain build, no sanitizers, for isolating sanitizer-only issues
plain: $(TARGET_PLAIN)

$(TARGET_PLAIN): $(PLAIN_OBJS)
	$(CXX) $(PLAIN_OBJS) -o $@ $(NOTCURSES_LIBS) -lncursesw

$(BUILD_DIR)-plain/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(BASE_CXXFLAGS) -O2 -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR)-plain $(TARGET) $(TARGET_PLAIN)

.PHONY: all clean plain
