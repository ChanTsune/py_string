#define BOOST_TEST_MAIN 

#include <iostream>
#include <vector>
#include <deque>

#include <boost/test/included/unit_test.hpp>

#include "py_string.hpp"
#include "tupleplus.hpp"

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

void test_str() {
    //    cout << "multiplication" << endl;
    py::string mul = "str";

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
    py::string cnt = mul * 10;
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

BOOST_AUTO_TEST_SUITE(pyUtil)

BOOST_AUTO_TEST_CASE(sign){
    using py::util::sign;

    BOOST_CHECK_EQUAL(sign(0), 0);
    BOOST_CHECK_EQUAL(sign(-2), -1);
    BOOST_CHECK_EQUAL(sign(3), 1);
}
BOOST_AUTO_TEST_CASE(optional){
    using py::optional_int;
    optional_int null = nullptr;
    optional_int i = 1;
    optional_int null2 = nullptr;

    BOOST_CHECK(null == null);
    BOOST_CHECK(null == null2);
    BOOST_CHECK(null == nullptr);
    BOOST_CHECK(null != i);
    BOOST_CHECK(i = i);

    BOOST_CHECK(i == 1);
    BOOST_CHECK(i != 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(pyString)

BOOST_AUTO_TEST_CASE(adjustIndex)
{
    using py::util::adjust_index;
    using std::make_tuple;

    BOOST_CHECK(adjust_index<int>(nullptr, nullptr, 1, 5) == make_tuple(0, 5, 1, 5));
    BOOST_CHECK(adjust_index<int>(nullptr, nullptr, 2, 5) == make_tuple(0, 5, 2, 3));
    BOOST_CHECK(adjust_index<int>(nullptr, nullptr, -1, 5) == make_tuple(4, -1, -1, 5));
    BOOST_CHECK(adjust_index<int>(1, 2, 3, 5) == make_tuple(1,2,3,1));
    BOOST_CHECK(adjust_index<int>(-5, -1, -2, 6) == make_tuple(1, 5, -2, 0));
}
BOOST_AUTO_TEST_CASE(slice)
{
    using py::string;
    string s = "01234";
    string t = "012345";

    BOOST_CHECK_EQUAL( (s[{nullptr, nullptr, -1}]), "43210");

    BOOST_CHECK_EQUAL( (s[{nullptr, nullptr, -2}]), "420");

    BOOST_CHECK_EQUAL( (t[{nullptr, nullptr, -1}]), "543210");

    BOOST_CHECK_EQUAL( (t[{nullptr, nullptr, -2}]), "531");

    BOOST_CHECK_EQUAL( (t[{-5, -1, 1}]), "1234");

    BOOST_CHECK_EQUAL( (s[{1, 5, -2}]), "");
}

BOOST_AUTO_TEST_CASE(repeat){
    using py::string;
    string str = "str";
    BOOST_CHECK_EQUAL(str * 2, "strstr");

    BOOST_CHECK_EQUAL(str * 0, "");

    BOOST_CHECK_EQUAL(str * 1, "str");

    str *= 2;
    BOOST_CHECK_EQUAL(str, "strstr");

    BOOST_CHECK_THROW(str * -1, std::length_error);
    /* Exceeds buffer length that can be held by string class */
}

BOOST_AUTO_TEST_CASE(arraySubscript)
{
    using py::string;
    string str = "012345";
    BOOST_CHECK_EQUAL(str[0], '0');
    BOOST_CHECK_EQUAL(str.at(0), '0');

    BOOST_CHECK_EQUAL(str[-1], '5');
    BOOST_CHECK_EQUAL(str.at(-1), '5');

    BOOST_CHECK_NO_THROW(str[-10]);
    BOOST_CHECK_THROW(str.at(-10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(capitalize){
    using py::string;
    string str = "str";
    string Str = "Str";
    string empty = "";
    
    BOOST_CHECK_EQUAL(str.capitalize(), "Str");
    BOOST_CHECK_EQUAL(Str.capitalize(), "Str");
    BOOST_CHECK_EQUAL(empty.capitalize(), "");
}

BOOST_AUTO_TEST_CASE(center){
    using py::string;
    string str = "string";
    string empty = "";

    BOOST_CHECK_EQUAL(str.center(10), "  string  ");
    BOOST_CHECK_EQUAL(str.center(6), "string");
    BOOST_CHECK_EQUAL(str.center(9), " string  ");
    BOOST_CHECK_EQUAL(str.center(0), "string");
    BOOST_CHECK_EQUAL(str.center(8,'Q'), "QstringQ");

    BOOST_CHECK_EQUAL(empty.center(10), "          ");
    BOOST_CHECK_EQUAL(empty.center(0), "");
    BOOST_CHECK_EQUAL(empty.center(8, 'e'), "eeeeeeee");
}

BOOST_AUTO_TEST_CASE(count){
    using py::string;
    string str = "str";
    string empty = "";
    string one = "a";
    string two = "ab";
    string three = "aba";

    BOOST_CHECK_EQUAL((str * 10).count("str"), 10);
    BOOST_CHECK_EQUAL(str.count(""), 4);
    BOOST_CHECK_EQUAL(empty.count("a"), 0);
    BOOST_CHECK_EQUAL(empty.count(""), 1);
    BOOST_CHECK_EQUAL((one * 10).count("a"), 10);
    BOOST_CHECK_EQUAL((two * 10).count("ab"), 10);
    BOOST_CHECK_EQUAL((two * 10).count("ba"), 9);
    BOOST_CHECK_EQUAL((three * 10).count("aba"), 10);
    BOOST_CHECK_EQUAL((three * 10).count("aa"), 9);

    BOOST_CHECK_EQUAL(empty.count("", 1, 2), 1);
    BOOST_CHECK_EQUAL(str.count("t", 1, 3), 1);
}

BOOST_AUTO_TEST_CASE(expandtabs){
    using py::string;
    string tab = "\t\t";
    string empty = "";
    string tab2 = "\t as\t";
    BOOST_CHECK_EQUAL(tab.expandtabs().size(), 16);
    BOOST_CHECK_EQUAL(tab.expandtabs(4).size(), 8);
    BOOST_CHECK_EQUAL(empty.expandtabs().size(), 0);
    BOOST_CHECK_EQUAL(tab2.expandtabs(4).size(), 11);
}

BOOST_AUTO_TEST_SUITE_END()
