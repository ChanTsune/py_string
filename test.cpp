#include "py_string.hpp"
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;
using std::cerr;

template <class T, class U>
int equal(T a, U b)
{
    if (a == b)
    {
        cout << "pass    : " << a << " == " << b << endl;
        return 0;
    }
    cerr << "Invalid : " << a << " != " << b << endl;
    return -1;
}
template <class T, class U>
int not_equal(T a, U b) {
    if (a != b) {
        cout << "pass    : " << a << " != " << b << endl;
        return 0;
    }
    cerr << "Invalid : " << a << " == " << b << endl;
    return -1;
}
int test_true(bool a){
    if (a) {
        cout << "pass    : true == " << a << endl;
        return 0;
    }
    cerr << "Invalid : true != " << a << endl;
    return -1;
}
int test_false(bool a){
    if (!a) {
        cout << "pass    : false == " << a << endl;
        return 0;
    }
    cerr << "Invalid : false != " << a << endl;
    return -1;
}

void test_str() {
    //    cout << "multiplication" << endl;
    py::string mul = "str";
    equal(mul * 2, "strstr");
    mul *= 2;
    equal(mul, "strstr");

    //    cout << "pyfind" << endl;
    equal(mul.pyfind("t"), 1);
    equal(mul.pyfind("t", 1), 1);
    equal(mul.pyfind("t", 2), 4);
    equal(mul.pyfind("t", 3), 4);
    equal(mul.pyfind("t", 4), 4);
    equal(mul.pyfind("t", -1), -1);
    equal(mul.pyfind("t", -2), 4);
    equal(mul.pyfind("t", -3), 4);
    equal(mul.pyfind("t", -4), 4);

    mul = "str";
    //    cout << "array subscription" << endl;
    equal(mul.at(-1), 'r');
    equal(mul[-1], 'r');

    //    cout << "capitalize" << endl;
    equal(mul.capitalize(), "Str");

    //    cout << "center" << endl;
    equal(mul.center(12, 'w'), "wwwwstrwwwww");

    //    cout << "count" << endl;
    py::string cnt = mul * 10;
    equal(cnt.count("str"), 10);
    equal(cnt.count("str", 3, -3), 8);
    equal(cnt.count("str", -10), 3);
    equal(cnt.count("str", -11), 3);
    equal(cnt.count("str", -12), 4);
    py::string aaa = "a";
    aaa *= 10;
    equal(aaa.count("a"), 10);

    //    cout << "endswish" << endl;
    equal(cnt.endswith("r"), true);
    equal(cnt.endswith("st", 0, -1), true);
    equal(cnt.endswith("t"), false);

    //    cout << "pyreplace" << endl;
    py::string rep = "abcdabcd";
    equal(rep.pyreplace("ab", "ABA"), "ABAcdABAcd");

    //    cout << "pyrfined" << endl;
    equal((mul * 2).pyrfind("st"), 3);

    //    cout << "startswish" << endl;

    equal(cnt.startswith("tr", 1), true);
    equal(cnt.startswith("str", 3), true);
    equal(cnt.startswith("str", 1), false);

    // join
    py::string sep = "-";
    vector<py::string> items = {"a", "b", "c"};
    equal(sep.join(items), "a-b-c");
    equal(sep.join(vector<py::string>{"a", "b", "c"}), "a-b-c");
}

void test_null_allow(){
    using py::optional_int;
    optional_int null = nullptr;
    optional_int i = 1;
    optional_int null2 = nullptr;

    equal(null,null);
    equal(null,null2);
    equal(null,nullptr);
    not_equal(null,i);
    equal(i,i);

    equal(i,1);
    not_equal(i,2);
}

int main(int argc, char const *argv[])
{
    test_null_allow();
    return 0;
}

