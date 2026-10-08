CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -MMD -MP
CPPFLAGS := -Iinclude
LDFLAGS :=
LDLIBS :=

TARGET := build/apv_simulator
SRC_DIR := src
BUILD_DIR := build

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPFILES := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	find $(BUILD_DIR) -maxdepth 1 -type f ! -name '.gitkeep' -delete 2>/dev/null || true

-include $(DEPFILES)