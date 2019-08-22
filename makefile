CXX = clang++
CXX_OPTIONS = -Wall --std=c++14


.PHONY:all

all:example

example:example.cpp py_string.hpp
	$(CXX) $(CXX_OPTIONS) $< -o $@

test:test.cpp py_string.hpp
	$(CXX) $(CXX_OPTIONS) $< -o $@
	./$@

clean:
	rm example