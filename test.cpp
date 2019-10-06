#define BOOST_TEST_MAIN 

#include <iostream>
#include <vector>
#include <deque>

#include <boost/test/included/unit_test.hpp>

#include "py_string.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::cerr;

namespace aux
{
template <std::size_t...>
struct seq
{
};

template <std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...>
{
};

template <std::size_t... Is>
struct gen_seq<0, Is...> : seq<Is...>
{
};

template <class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple(std::basic_ostream<Ch, Tr> &os, Tuple const &t, seq<Is...>)
{
    using swallow = int[];
    (void)swallow{0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...};
}
} // namespace aux

template <class Ch, class Tr, class... Args>
auto operator<<(std::basic_ostream<Ch, Tr> &os, std::tuple<Args...> const &t)
    -> std::basic_ostream<Ch, Tr> &
{
    os << "(";
    aux::print_tuple(os, t, aux::gen_seq<sizeof...(Args)>());
    return os << ")";
}

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

BOOST_AUTO_TEST_SUITE(pyUtil)

BOOST_AUTO_TEST_CASE(sign){
    using py::util::sign;

    BOOST_CHECK_EQUAL(sign(0), 0);
    BOOST_CHECK_EQUAL(sign(-2), -1);
    BOOST_CHECK_EQUAL(sign(3), 1);
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

BOOST_AUTO_TEST_SUITE_END()
