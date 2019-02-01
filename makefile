CXX = clang++
CXX_OPTIONS = -Wall --std=c++14


.PHONY:all

all:example

example:example.cpp
	$(CXX) $(CXX_OPTIONS) $^ -o $@

test:test.cpp
	$(CXX) $(CXX_OPTIONS) $^ -o $@
	./$@

clean:
	rm example