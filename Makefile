SRC_DIR = src
BUILD_DIR = build
TEST_DIR = test
TEST_FILE = tests.txt
TARGET = $(BUILD_DIR)/tcpcount

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lpcap
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

test: $(TARGET)
	cd $(TEST_DIR) && ./run_tests.sh $(abspath $<) $(TEST_FILE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean
