#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "py_string.hpp"



using namespace std;

int main(int argc, char const *argv[])
{

    //string multiplication
    py::string str = "hello world!";

    std::cout << str * 2 << std::endl;
    //hello world!hello world!


    //string slice
    py::string sstr = "0123456789";

    cout << sstr.slice(0,20,2) << endl;
    cout << sstr.slice(0, 20, -2) << endl;
    cout << sstr[{4,nullptr}] << endl;

    //string format
    py::string fstr = "{} is {} than {}";

    cout << fstr.format(3,"bigger",1) << endl;

    cout << fstr.format(2,"smaller",4) << endl;

    //string split
    py::string spstr = "12,3,4-2,2,3";
    std::deque<py::string> slist; //std::vector is also acceptable

    spstr.split(slist, ",");

    for (auto &&s : slist)
    {
        cout << s << endl;
    }

    //translate
    py::string tstr = "text abc so cool.";
    auto table = py::string::maketrans("abcd","ABCD",".");
    std::map<py::string,py::string> _t;
    _t["."]="!!";
    _t["s"]="";
    _t["o"]="O";

    auto table2 = py::string::maketrans(_t);

    cout << tstr.translate(table) << endl;

    cout << tstr.translate(table2) << endl; 

    return 0;
}
