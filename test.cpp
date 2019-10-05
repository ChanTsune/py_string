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

void test_sing() {
    using py::util::sign;

    equal(sign(0),0);
    equal(sign(-2),-1);
    equal(sign(3),1);
}

void test_adjust_index() {
    cout << py::util::adjust_index<int>(nullptr, nullptr, 1, 5) << endl;
    cout << py::util::adjust_index<int>(nullptr, nullptr, 2, 5) << endl;
    cout << py::util::adjust_index<int>(nullptr, nullptr, -1, 5) << endl;
    cout << py::util::adjust_index<int>(1, 2, 3, 5) << endl;
    cout << py::util::adjust_index<int>(-5, -1, -2, 6) << endl;
    py::string s = "01234";
    py::string t = "012345";
    cout << s[{nullptr, nullptr, -1}] << endl;
    cout << s[{nullptr, nullptr, -2}] << endl;
    cout << t[{nullptr, nullptr, -1}] << endl;
    cout << t[{nullptr, nullptr, -2}] << endl;
    cout << t[{-5,-1,1}] << endl;
    cout << s[{1,5,-2}] << endl;

}

BOOST_AUTO_TEST_SUITE(pyString)

BOOST_AUTO_TEST_CASE(adjustIndex)
{
    BOOST_CHECK_EQUAL(2*2, 4);
}

BOOST_AUTO_TEST_CASE(slice)
{
    BOOST_CHECK_EQUAL(2*3, 6);
}

BOOST_AUTO_TEST_SUITE_END()
