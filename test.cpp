#include "py_string.hpp"
#include <iostream>

template<class T,class U>
int equal(T a,U b)
{
    if(a == b)
    {
    std::cout << "pass    : " << a << " == " << b << std::endl;
        return 0;
    }
    std::cerr << "Invalid : " << a << " != " << b << std::endl;
    return -1;
}

int main(int argc, char const *argv[])
{
    py::string mul = "str";
    equal(mul*2,"strstr");
    equal(mul.pyfind("t"),1);
    equal(mul.at(-1),'r');
    equal(mul[-1],'r');
    equal(mul.capitalize(),"Str");
    equal(mul.center(12,'w'),"wwwwstrwwwww");
    py::string cnt = mul * 10;
    equal(cnt.count("str"),10);
    equal(cnt.count("str",3,-3),8);
    equal(cnt.endswith("r"),true);
    equal(cnt.endswith("st",0,-1),true);
    equal(cnt.endswith("t"),false);
    py::string rep = "abcdabcd";
    equal(rep.pyreplace("ab","ABA"),"ABAcdABAcd");
    equal((mul*2).pyrfind("st"),3);
    equal(cnt.startswith("tr",1),true);
    equal(cnt.startswith("str",3),true);
    equal(cnt.startswith("str",1),false);

    return 0;
}