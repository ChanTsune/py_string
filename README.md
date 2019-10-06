# py_string

[![Build Status](https://travis-ci.org/ChanTsune/py_string.svg?branch=master)](https://travis-ci.org/ChanTsune/py_string)

A library that provides methods equivalent to Python 3's str class in C ++  
A class that reproduced the Python str class

## Installation

Since this is a single header library, it is easy to use by include only.

## How to use

Since it inherits std::string, it can be handled like std::string. Think that you can use python's str type method with std::string.

```cpp
#include "py_string.hpp"
```

### multipication

```cpp
py::string str = "hello world!";

std::cout << str*2 << std::endl;
//hello world!hello world!
```

### formatting for title style

```cpp
std::cout << str.title() << std::endl;
//Hello World!
```

### splitting

```cpp
py::string str2 = "12,3,4-2,2,3";
std::deque<py::string> slist; //std::vector is also acceptable

str2.split(slist, ",");

for (auto &&s : slist)
{
    std::cout << s << std::endl;
}
//12
//3
//4-2
//2
//3
```

### formatting

```cpp
py::string str3 = "{0} format string{1}";
std::cout << str3.format("let's","!") << std::endl;
//let's format string!
py::string str4 = "{}{}{}";
std::cout << str4.format("1",2,"3") << std::endl;
//123

py::string str = "{0},{1},{0}";
std::cout << str.format("apple","banana") << std::endl;
//apple,banana,apple

```

### string slicing

```cpp
py::string str5 = "0123456789";
std::cout << str5[{0,5}] << std::endl;
//01234
std::cout << str5[{nullptr,nullptr,-1}] << std::end;
//9876543210
```

### reverse order access

```cpp
std::cout << str5[-1] << std::endl;
//9
std::cout << str5[-3] << std::endl;
//7
```

For more detailed usage of this library, please refer to example.cpp.

The behavior of each method is almost the same as that of python's str type.
Please see python's official support for details.

https://docs.python.org/3/library/stdtypes.html#string-methods  
