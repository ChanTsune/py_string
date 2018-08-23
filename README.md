# py_string
A class that reproduced the Python str type in C ++

## Installation
Since this is a single header library, it is easy to use by include only.

## How to use
Since it inherits std::string, it can be handled like std::string. Think that you can use python's str type method with std::string.

```cpp
py::string str = "hello world!";

std::cout << str*2 << std::endl;
//hello world!hello world!
```

```cpp
std::cout << str.title() << std::endl;
//Hello World!
```


For more detailed usage of this library please refer to example.cpp.

The behavior of each method is almost the same as that of python's str type.
Please see python's official support for details.

https://docs.python.org/3/library/stdtypes.html#string-methods