CXX := g++
AR := ar
NOTCURSES_CFLAGS := $(shell pkg-config --cflags notcurses-core)
NOTCURSES_LIBS := $(shell pkg-config --libs notcurses-core)
GTEST_CFLAGS := $(shell pkg-config --cflags gtest gmock)
GTEST_LIBS := $(shell pkg-config --libs gtest gmock gtest_main)
# Shared flags (no sanitizers here)
BASE_CXXFLAGS := -Wall -Wextra -std=c++23 -g -Isrc/Headers $(NOTCURSES_CFLAGS)
# emit .d files so header edits trigger rebuilds of dependent objects
DEPFLAGS := -MMD -MP
SAN_FLAGS := -fsanitize=address -fsanitize=undefined
CXXFLAGS := $(BASE_CXXFLAGS) -O1 $(SAN_FLAGS)
LDFLAGS := $(SAN_FLAGS)
LDLIBS := $(NOTCURSES_LIBS) -lncursesw
# tests see src/ (so they can write #include "GameSpecific/...") and the support code
TEST_INCLUDES := -Isrc -Itests/support $(GTEST_CFLAGS)
SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build
BUILD_DIR_DEBUG := build-debug
HEADER_DIR := src/Headers

# Engine = everything except the game and the entry point; game = src/GameSpecific.
ALL_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
GAME_SRCS := $(filter $(SRC_DIR)/GameSpecific/%,$(ALL_SRCS))
MAIN_SRC := $(SRC_DIR)/TextQuest.cpp
ENGINE_SRCS := $(filter-out $(GAME_SRCS) $(MAIN_SRC),$(ALL_SRCS))

TEST_SUPPORT_SRCS := $(shell find $(TEST_DIR)/support -name '*.cpp' 2>/dev/null)
ENGINE_TEST_SRCS := $(shell find $(TEST_DIR)/engine $(TEST_DIR)/structure -name '*.cpp' 2>/dev/null)
GAME_TEST_SRCS := $(shell find $(TEST_DIR)/game -name '*.cpp' 2>/dev/null)

# $(call objs,<build dir>,<sources>)
objs = $(patsubst %.cpp,$(1)/%.o,$(2))

TARGET := tq
TARGET_DEBUG := tq-debug

# plain build
ENGINE_LIB := $(BUILD_DIR)/libtqengine.a
GAME_LIB := $(BUILD_DIR)/libtqgame.a
MAIN_OBJ := $(call objs,$(BUILD_DIR),$(MAIN_SRC))
ENGINE_TEST_BIN := $(BUILD_DIR)/tests/tq-engine-tests
GAME_TEST_BIN := $(BUILD_DIR)/tests/tq-game-tests

# sanitized build
ENGINE_LIB_DBG := $(BUILD_DIR_DEBUG)/libtqengine.a
GAME_LIB_DBG := $(BUILD_DIR_DEBUG)/libtqgame.a
MAIN_OBJ_DBG := $(call objs,$(BUILD_DIR_DEBUG),$(MAIN_SRC))
ENGINE_TEST_BIN_DBG := $(BUILD_DIR_DEBUG)/tests/tq-engine-tests
GAME_TEST_BIN_DBG := $(BUILD_DIR_DEBUG)/tests/tq-game-tests

ENGINE_OBJS := $(call objs,$(BUILD_DIR),$(ENGINE_SRCS))
GAME_OBJS := $(call objs,$(BUILD_DIR),$(GAME_SRCS))
ENGINE_OBJS_DBG := $(call objs,$(BUILD_DIR_DEBUG),$(ENGINE_SRCS))
GAME_OBJS_DBG := $(call objs,$(BUILD_DIR_DEBUG),$(GAME_SRCS))

SUPPORT_OBJS := $(call objs,$(BUILD_DIR),$(TEST_SUPPORT_SRCS))
ENGINE_TEST_OBJS := $(call objs,$(BUILD_DIR),$(ENGINE_TEST_SRCS))
GAME_TEST_OBJS := $(call objs,$(BUILD_DIR),$(GAME_TEST_SRCS))
SUPPORT_OBJS_DBG := $(call objs,$(BUILD_DIR_DEBUG),$(TEST_SUPPORT_SRCS))
ENGINE_TEST_OBJS_DBG := $(call objs,$(BUILD_DIR_DEBUG),$(ENGINE_TEST_SRCS))
GAME_TEST_OBJS_DBG := $(call objs,$(BUILD_DIR_DEBUG),$(GAME_TEST_SRCS))

ALL_OBJS := $(ENGINE_OBJS) $(GAME_OBJS) $(MAIN_OBJ) $(SUPPORT_OBJS) \
            $(ENGINE_TEST_OBJS) $(GAME_TEST_OBJS) \
            $(ENGINE_OBJS_DBG) $(GAME_OBJS_DBG) $(MAIN_OBJ_DBG) $(SUPPORT_OBJS_DBG) \
            $(ENGINE_TEST_OBJS_DBG) $(GAME_TEST_OBJS_DBG)

all: $(TARGET)

# --- plain build (default, no sanitizers) ---
# Link order matters for static libs: game depends on engine, so engine comes last.
$(TARGET): $(MAIN_OBJ) $(GAME_LIB) $(ENGINE_LIB)
	$(CXX) $(MAIN_OBJ) $(GAME_LIB) $(ENGINE_LIB) -o $@ $(NOTCURSES_LIBS) -lncursesw
$(ENGINE_LIB): $(ENGINE_OBJS)
	rm -f $@ && $(AR) rcs $@ $^
$(GAME_LIB): $(GAME_OBJS)
	rm -f $@ && $(AR) rcs $@ $^

$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(BASE_CXXFLAGS) $(DEPFLAGS) -O2 -c $< -o $@
$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(BASE_CXXFLAGS) $(TEST_INCLUDES) $(DEPFLAGS) -O2 -c $< -o $@

# --- sanitized build, for isolating sanitizer-only issues ---
debug: $(TARGET_DEBUG)
$(TARGET_DEBUG): $(MAIN_OBJ_DBG) $(GAME_LIB_DBG) $(ENGINE_LIB_DBG)
	$(CXX) $(MAIN_OBJ_DBG) $(GAME_LIB_DBG) $(ENGINE_LIB_DBG) -o $@ $(LDFLAGS) $(LDLIBS)
$(ENGINE_LIB_DBG): $(ENGINE_OBJS_DBG)
	rm -f $@ && $(AR) rcs $@ $^
$(GAME_LIB_DBG): $(GAME_OBJS_DBG)
	rm -f $@ && $(AR) rcs $@ $^

$(BUILD_DIR_DEBUG)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@
$(BUILD_DIR_DEBUG)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) $(DEPFLAGS) -c $< -o $@

# --- tests ---
# The engine tests link against the engine lib only: a link error here means the
# engine depends on game code.
$(ENGINE_TEST_BIN): $(ENGINE_TEST_OBJS) $(SUPPORT_OBJS) $(ENGINE_LIB)
	$(CXX) $^ -o $@ $(GTEST_LIBS) $(NOTCURSES_LIBS) -lncursesw
$(GAME_TEST_BIN): $(GAME_TEST_OBJS) $(SUPPORT_OBJS) $(GAME_LIB) $(ENGINE_LIB)
	$(CXX) $^ -o $@ $(GTEST_LIBS) $(NOTCURSES_LIBS) -lncursesw
$(ENGINE_TEST_BIN_DBG): $(ENGINE_TEST_OBJS_DBG) $(SUPPORT_OBJS_DBG) $(ENGINE_LIB_DBG)
	$(CXX) $^ -o $@ $(LDFLAGS) $(GTEST_LIBS) $(LDLIBS)
$(GAME_TEST_BIN_DBG): $(GAME_TEST_OBJS_DBG) $(SUPPORT_OBJS_DBG) $(GAME_LIB_DBG) $(ENGINE_LIB_DBG)
	$(CXX) $^ -o $@ $(LDFLAGS) $(GTEST_LIBS) $(LDLIBS)

# Run from the repo root: Logger opens ./logs/*.log at static init.
test: $(ENGINE_TEST_BIN) $(GAME_TEST_BIN)
	@mkdir -p logs
	@status=0; \
	./$(ENGINE_TEST_BIN) || status=1; \
	./$(GAME_TEST_BIN) || status=1; \
	exit $$status

test-debug: $(ENGINE_TEST_BIN_DBG) $(GAME_TEST_BIN_DBG)
	@mkdir -p logs
	@status=0; \
	./$(ENGINE_TEST_BIN_DBG) || status=1; \
	./$(GAME_TEST_BIN_DBG) || status=1; \
	exit $$status

-include $(ALL_OBJS:.o=.d)

clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR_DEBUG) $(TARGET) $(TARGET_DEBUG)
.PHONY: all clean debug test test-debug
