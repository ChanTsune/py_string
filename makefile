SHELL := /bin/bash
ifdef CXX
else
	CXX = clang++
endif
ifdef CXX_VERSION
else
	CXX_VERSION = c++14
endif

CXX_OPTIONS = -Wall --std=${CXX_VERSION}

BUILD_DIR = build

.PHONY:all

all:example test

example:example.cpp py_string.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXX_OPTIONS) $< -o $(BUILD_DIR)/$@
	./$(BUILD_DIR)/$@

clean:
	rm $(BUILD_DIR)/example $(BUILD_DIR)/test

test:test.cpp py_string.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXX_OPTIONS) $< -o $(BUILD_DIR)/$@
	./$(BUILD_DIR)/$@
