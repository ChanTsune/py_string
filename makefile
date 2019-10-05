ifdef CXX
else
	CXX = clang++
endif
CXX_OPTIONS = -Wall --std=c++14

BUILD_DIR = build

.PHONY:all

all:example test

example:example.cpp py_string.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXX_OPTIONS) $< -o $(BUILD_DIR)/$@
	./$(BUILD_DIR)/$@

test:test.cpp py_string.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXX_OPTIONS) $< -o $(BUILD_DIR)/$@
	./$(BUILD_DIR)/$@

clean:
	rm $(BUILD_DIR)/example $(BUILD_DIR)/test

googletest_install:
	git clone https://github.com/google/googletest.git
	cd googletest
	mkdir build && cd build
	cmake ..
	make
	cd ../../
	sudo cp -r ./googletest/googlemock/include/gmock /usr/local/include/gmock
	sudo cp -r ./googletest/googletest/include/gtest /usr/local/include/gtest
	sudo cp ./googletest/build/lib/*.a /usr/local/lib/

googletest:test.cpp py_string.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXX_OPTIONS) $< -o $(BUILD_DIR)/$@
	./$(BUILD_DIR)/$@
