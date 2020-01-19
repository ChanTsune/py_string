#define BOOST_TEST_MAIN

#include <vector>
#include <deque>

#include <boost/test/included/unit_test.hpp>

#include "../py_string.hpp"
#include "tupleplus.hpp"

BOOST_AUTO_TEST_SUITE(pyUtil)

BOOST_AUTO_TEST_CASE(sign)
{
    using py::util::sign;

    BOOST_CHECK_EQUAL(sign(0), 0);
    BOOST_CHECK_EQUAL(sign(-2), -1);
    BOOST_CHECK_EQUAL(sign(3), 1);
}
BOOST_AUTO_TEST_CASE(optional)
{
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
    BOOST_CHECK(adjust_index<int>(1, 2, 3, 5) == make_tuple(1, 2, 3, 1));
    BOOST_CHECK(adjust_index<int>(-5, -1, -2, 6) == make_tuple(1, 5, -2, 0));
}
BOOST_AUTO_TEST_CASE(slice)
{
    using py::string;
    string s = "01234";
    string t = "012345";

    BOOST_CHECK_EQUAL((s[{nullptr, nullptr, -1}]), "43210");

    BOOST_CHECK_EQUAL((s[{nullptr, nullptr, -2}]), "420");

    BOOST_CHECK_EQUAL((t[{nullptr, nullptr, -1}]), "543210");

    BOOST_CHECK_EQUAL((t[{nullptr, nullptr, -2}]), "531");

    BOOST_CHECK_EQUAL((t[{-5, -1, 1}]), "1234");

    BOOST_CHECK_EQUAL((s[{1, 5, -2}]), "");
}

BOOST_AUTO_TEST_CASE(repeat)
{
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

BOOST_AUTO_TEST_CASE(capitalize)
{
    using py::string;
    string str = "str";
    string Str = "Str";
    string empty = "";

    BOOST_CHECK_EQUAL(str.capitalize(), "Str");
    BOOST_CHECK_EQUAL(Str.capitalize(), "Str");
    BOOST_CHECK_EQUAL(empty.capitalize(), "");
}

BOOST_AUTO_TEST_CASE(center)
{
    using py::string;
    string str = "string";
    string empty = "";

    BOOST_CHECK_EQUAL(str.center(10), "  string  ");
    BOOST_CHECK_EQUAL(str.center(6), "string");
    BOOST_CHECK_EQUAL(str.center(9), " string  ");
    BOOST_CHECK_EQUAL(str.center(0), "string");
    BOOST_CHECK_EQUAL(str.center(8, 'Q'), "QstringQ");

    BOOST_CHECK_EQUAL(empty.center(10), "          ");
    BOOST_CHECK_EQUAL(empty.center(0), "");
    BOOST_CHECK_EQUAL(empty.center(8, 'e'), "eeeeeeee");
}

BOOST_AUTO_TEST_CASE(count)
{
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

BOOST_AUTO_TEST_CASE(expandtabs)
{
    using py::string;
    string tab = "\t\t";
    string empty = "";
    string tab2 = "\t as\t";
    BOOST_CHECK_EQUAL(tab.expandtabs().size(), 16);
    BOOST_CHECK_EQUAL(tab.expandtabs(4).size(), 8);
    BOOST_CHECK_EQUAL(empty.expandtabs().size(), 0);
    BOOST_CHECK_EQUAL(tab2.expandtabs(4).size(), 11);
}

BOOST_AUTO_TEST_CASE(endswith)
{
    using py::string;

    string str = "strstr";

    BOOST_CHECK(str.endswith("str"));
    BOOST_CHECK(str.endswith("st", 0, -1));
    BOOST_CHECK(str.endswith("str", 0, -3));
    BOOST_CHECK(!str.endswith("str", 0, -1));
    BOOST_CHECK(!str.endswith("str", 0, 1));
}

BOOST_AUTO_TEST_CASE(find)
{
    using py::string;
    string str = "strstr";

    BOOST_CHECK_EQUAL(str.pyfind("t"), 1);
    BOOST_CHECK_EQUAL(str.pyfind("t", 1), 1);
    BOOST_CHECK_EQUAL(str.pyfind("t", 2), 4);
    BOOST_CHECK_EQUAL(str.pyfind("t", 3), 4);
    BOOST_CHECK_EQUAL(str.pyfind("t", 4), 4);
    BOOST_CHECK_EQUAL(str.pyfind("t", -1), -1);
    BOOST_CHECK_EQUAL(str.pyfind("t", -2), 4);
    BOOST_CHECK_EQUAL(str.pyfind("t", -3), 4);
    BOOST_CHECK_EQUAL(str.pyfind("t", -4), 4);

    BOOST_CHECK_EQUAL(str.pyfind(""), 0);
    BOOST_CHECK_EQUAL(str.pyfind("", 1), 1);
}

BOOST_AUTO_TEST_CASE(join_with_vector)
{
    using py::string;
    using std::vector;

    string sep = "-";
    string empty = "";
    vector<string> items = {"a", "b", "c"};
    vector<string> no_item = {};

    BOOST_CHECK_EQUAL(sep.join(items), "a-b-c");
    BOOST_CHECK_EQUAL(sep.join(vector<string>{"a", "b", "c"}), "a-b-c");
    BOOST_CHECK_EQUAL(sep.join(no_item), "");
    BOOST_CHECK_EQUAL(empty.join(items), "abc");
    BOOST_CHECK_EQUAL(empty.join(no_item), "");
}

BOOST_AUTO_TEST_CASE(join_with_deque)
{
    using py::string;
    using std::deque;

    string sep = "-";
    deque<string> items = {"a", "b", "c"};
    BOOST_CHECK_EQUAL(sep.join(items), "a-b-c");
}

BOOST_AUTO_TEST_CASE(ljust)
{
    using py::string;
    string str = "str";
    string empty = "";
    BOOST_CHECK_EQUAL(str.ljust(10), "str       ");
    BOOST_CHECK_EQUAL(str.ljust(10, '&'), "str&&&&&&&");
    BOOST_CHECK_EQUAL(str.ljust(2), "str");
    BOOST_CHECK_EQUAL(empty.ljust(5), "     ");
    BOOST_CHECK_EQUAL(empty.ljust(5, 'f'), "fffff");
}

BOOST_AUTO_TEST_CASE(lower)
{
    using py::string;
    string upper = "STRING";
    string lower = "string";
    string empty = "";

    BOOST_CHECK_EQUAL(upper.lower(), lower);
    BOOST_CHECK_EQUAL(lower.lower(), lower);
    BOOST_CHECK_EQUAL(empty.lower(), empty);
}
BOOST_AUTO_TEST_CASE(lstrip)
{
    using py::string;
    string spacious = "    spacious    ";
    string domain = "www.example.com";
    string empty = "";
    BOOST_CHECK_EQUAL(spacious.lstrip(), "spacious    ");
    BOOST_CHECK_EQUAL(domain.lstrip("comwz."), "example.com");
    BOOST_CHECK_EQUAL(domain.lstrip(""), domain);
    BOOST_CHECK_EQUAL(empty.lstrip(), "");
    BOOST_CHECK_EQUAL(empty.lstrip(""), "");
}

BOOST_AUTO_TEST_CASE(partition)
{
    using py::string;
    using std::vector;
    string str = "1-2-3-4-5";
    vector<string> tmp = {};
    str.partition("-", tmp);

    BOOST_CHECK(tmp == (vector<string>{"1", "-", "2-3-4-5"}));
    str.partition("p", tmp);
    BOOST_CHECK(tmp == (vector<string>{"1-2-3-4-5", "", ""}));
}

BOOST_AUTO_TEST_CASE(replace)
{
    using py::string;
    string nice_language = "Python is nice programming language!";
    string str = "aaaaaaaaaa";

    BOOST_CHECK_EQUAL(nice_language.pyreplace("Python", "C++"), "C++ is nice programming language!");
    BOOST_CHECK_EQUAL(str.pyreplace("a", "A", 5), "AAAAAaaaaa");
}

BOOST_AUTO_TEST_CASE(rfind)
{
    using py::string;
    string str = "strstr";

    BOOST_CHECK_EQUAL(str.pyrfind("tr"), 4);
    BOOST_CHECK_EQUAL(str.pyrfind("tr", 0, -1), 1);
    BOOST_CHECK_EQUAL(str.pyrfind(""), 6);
}

BOOST_AUTO_TEST_CASE(rjust)
{
    using py::string;

    string str = "str";
    string empty = "";
    BOOST_CHECK_EQUAL(str.rjust(10), "       str");
    BOOST_CHECK_EQUAL(str.rjust(10, '&'), "&&&&&&&str");
    BOOST_CHECK_EQUAL(str.rjust(2), "str");
    BOOST_CHECK_EQUAL(empty.rjust(5), "     ");
    BOOST_CHECK_EQUAL(empty.rjust(5, 'f'), "fffff");
}

BOOST_AUTO_TEST_CASE(rpartition)
{
    using py::string;
    using std::vector;

    string str = "1-2-3-4-5";
    vector<string> tmp = {};
    str.rpartition("-", tmp);

    BOOST_CHECK(tmp == (vector<string>{"1-2-3-4", "-", "5"}));
    str.rpartition("p", tmp);
    BOOST_CHECK(tmp == (vector<string>{"", "", "1-2-3-4-5"}));
}

BOOST_AUTO_TEST_CASE(rsplit)
{
    using py::string;
    using std::vector;
    string str = "1,1,1,1,1,";
    string space = "1 1 1 1 1";
    string empty = "";
    vector<string> tmp = {};
    vector<string> ep = {};

    empty.rsplit(tmp);
    BOOST_CHECK(tmp == ep);

    str.rsplit(tmp, ",");
    BOOST_CHECK(tmp == (vector<string>{"1", "1", "1", "1", "1", ""}));

    str.rsplit(tmp, ",", 2);
    BOOST_CHECK(tmp == (vector<string>{"1,1,1,1", "1", ""}));

    space.rsplit(tmp);
    BOOST_CHECK(tmp == (vector<string>{"1", "1", "1", "1", "1"}));

    space.rsplit(tmp, 2);
    BOOST_CHECK(tmp == (vector<string>{"1 1 1", "1", "1"}));
}

BOOST_AUTO_TEST_CASE(rstrip)
{
    using py::string;
    string spacious = "    spacious    ";
    string domain = "www.example.com";
    string empty = "";
    BOOST_CHECK_EQUAL(spacious.rstrip(), "    spacious");
    BOOST_CHECK_EQUAL(domain.rstrip("comwz."), "www.example");
    BOOST_CHECK_EQUAL(domain.rstrip(""), domain);
    BOOST_CHECK_EQUAL(empty.rstrip(), "");
    BOOST_CHECK_EQUAL(empty.rstrip(""), "");
}

BOOST_AUTO_TEST_CASE(split)
{
    using py::string;
    using std::vector;
    string str = "1,1,1,1,1,";
    string space = "1 1 1 1 1";
    string empty = "";
    vector<string> tmp = {};
    vector<string> ep = {};

    empty.split(tmp);
    BOOST_CHECK(tmp == ep);

    str.split(tmp, ",");
    BOOST_CHECK(tmp == (vector<string>{"1", "1", "1", "1", "1", ""}));

    str.split(tmp, ",", 2);
    BOOST_CHECK(tmp == (vector<string>{"1", "1", "1,1,1,"}));

    space.split(tmp);
    BOOST_CHECK(tmp == (vector<string>{"1", "1", "1", "1", "1"}));

    space.split(tmp, 2);
    BOOST_CHECK(tmp == (vector<string>{"1", "1", "1 1 1"}));
}

BOOST_AUTO_TEST_CASE(splitlines)
{
    using py::string;
    using std::vector;

    string lines = "Python\nC++";
    string lines_end = "Python\nC++\n";
    vector<string> tmp = {};

    lines.splitlines(tmp);
    BOOST_CHECK(tmp == (vector<string>{"Python", "C++"}));

    lines.splitlines(tmp, true);
    BOOST_CHECK(tmp == (vector<string>{"Python\n", "C++"}));

    lines_end.splitlines(tmp);
    BOOST_CHECK(tmp == (vector<string>{"Python", "C++"}));

    lines_end.splitlines(tmp, true);
    BOOST_CHECK(tmp == (vector<string>{"Python\n", "C++\n"}));
}

BOOST_AUTO_TEST_CASE(startswith)
{
    using py::string;

    string str = "strstr";

    BOOST_CHECK(str.startswith("str"));
    BOOST_CHECK(str.startswith("tr", 1));
    BOOST_CHECK(str.startswith("str", 3));
    BOOST_CHECK(!str.startswith("str", 1));
    BOOST_CHECK(!str.startswith("str", 0, 1));
}

BOOST_AUTO_TEST_CASE(strip)
{
    using py::string;

    string spacious = "    spacious    ";
    string domain = "www.example.com";
    string empty = "";
    BOOST_CHECK_EQUAL(spacious.strip(), "spacious");
    BOOST_CHECK_EQUAL(domain.strip("comwz."), "example");
    BOOST_CHECK_EQUAL(domain.strip(""), domain);
    BOOST_CHECK_EQUAL(empty.strip(), "");
    BOOST_CHECK_EQUAL(empty.strip(""), "");
}

BOOST_AUTO_TEST_CASE(swapcase)
{
    using py::string;

    string str = "String";
    string str2 = "py_string";

    BOOST_CHECK_EQUAL(str.swapcase(), "sTRING");
    BOOST_CHECK_EQUAL(str2.swapcase(), "PY_STRING");
}

BOOST_AUTO_TEST_CASE(title)
{
    using py::string;

    string str = "string is like vector<char>";

    BOOST_CHECK_EQUAL(str.title(), "String Is Like Vector<Char>");
}

BOOST_AUTO_TEST_CASE(upper)
{
    using py::string;

    string upper = "STRING";
    string lower = "string";
    string empty = "";

    BOOST_CHECK_EQUAL(upper.upper(), upper);
    BOOST_CHECK_EQUAL(lower.upper(), upper);
    BOOST_CHECK_EQUAL(empty.upper(), empty);
}

BOOST_AUTO_TEST_CASE(zfill)
{
    using py::string;

    string b = "100";
    string c = "-100";
    string d = "+100";
    string e = "=100";
    string empty = "";

    BOOST_CHECK_EQUAL(b.zfill(5), "00100");
    BOOST_CHECK_EQUAL(b.zfill(3), "100");
    BOOST_CHECK_EQUAL(b.zfill(2), "100");
    BOOST_CHECK_EQUAL(c.zfill(5), "-0100");
    BOOST_CHECK_EQUAL(c.zfill(4), "-100");
    BOOST_CHECK_EQUAL(c.zfill(3), "-100");
    BOOST_CHECK_EQUAL(c.zfill(2), "-100");
    BOOST_CHECK_EQUAL(d.zfill(5), "+0100");
    BOOST_CHECK_EQUAL(e.zfill(5), "0=100");
    BOOST_CHECK_EQUAL(empty.zfill(5), "00000");
}

BOOST_AUTO_TEST_SUITE_END()
