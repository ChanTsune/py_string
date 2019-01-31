#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "py_string.hpp"

using namespace std;

int main(int argc, char const *argv[])
{

    //string multiplication
    py::string multiplication = "hello world!";

    std::cout << multiplication * 2 << std::endl;
    //hello world!hello world!

    //Subscript operator
    py::string Subscript_operator = "0123456789";

    cout << Subscript_operator[0] << endl;

    cout << Subscript_operator[-1] << endl; //last element

    cout << Subscript_operator[-3] << endl; //third element from the back (7)

    //string slice
    py::string slice = "0123456789";

    cout << slice.slice(0, 20, 2) << endl;
    cout << slice.slice(0, 20, -2) << endl;
    cout << slice[{4, nullptr}] << endl;

    //string format
    py::string format = "{} is {} than {}";

    cout << format.format(3, "bigger", 1) << endl;

    cout << format.format(2, "smaller", 4) << endl;

    py::string format2 = "{0} is {1} as {0}";

    cout << format2.format(2, "same") << endl;

    //string replace

    py::string replace = "1+2-3";

    cout << replace.pyreplace("-", "=");
    //1+2=3

    //string split
    py::string split = "12,3,4-2,2,3";
    std::deque<py::string> slist; //std::vector is also acceptable

    split.split(slist, ",");

    for (auto &&s : slist)
    {
        cout << s << endl;
    }
    // Check ASCII character string
    py::string ascii = "Hello World!";
    cout << std::boolalpha << ascii.isascii() << endl;

    py::string not_ascii = "ハローワールド！";
    cout << std::boolalpha << not_ascii.isascii() << endl;

    return 0;
}
