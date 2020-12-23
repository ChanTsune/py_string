#pragma once
#ifndef PY_STRING_HPP
#define PY_STRING_HPP

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cwctype>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <limits>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>

namespace py {
namespace null_allow {
  template <class T> class null_allow {
  private:
    T m_value;
    bool m_has_value;

  public:
    null_allow<T>(std::nullptr_t _Null)
      : m_has_value(false)
    {
    }
    null_allow<T>(T _Value)
      : m_value(_Value)
      , m_has_value(true)
    {
    }
    null_allow<T> &operator=(std::nullptr_t _Null)
    {
      m_has_value = false;
      return *this;
    }
    null_allow<T> &operator=(T _Value)
    {
      m_value = _Value;
      m_has_value = true;
      return *this;
    }
    bool operator==(null_allow<T> rhs) const
    {
      return (m_has_value && rhs.m_has_value)
               ? (m_value == rhs.m_value)
               : (m_has_value == rhs.m_has_value);
    };
    bool operator!=(null_allow<T> rhs) const
    {
      return !(this->operator==(rhs));
    };
    // bool operator<(null_allow<T> rhs){};
    // bool operator>(null_allow<T> rhs){};
    // bool operator<=(null_allow<T> rhs){};
    // bool operator>=(null_allow<T> rhs){};

    bool operator==(std::nullptr_t _Null) const { return !m_has_value; }
    bool operator!=(std::nullptr_t _Null) const { return m_has_value; }
    //  bool operator<(std::nullptr_t _Null){ return !m_has_value; }
    //  bool operator>(std::nullptr_t _Null){ return !m_has_value; }
    //  bool operator<=(std::nullptr_t _Null){ return m_has_value; }
    //  bool operator>=(std::nullptr_t _Null){ return m_has_value; }
    bool operator==(T &&_Value) const { return m_value == _Value; }
    bool operator!=(T &&_Value) const { return m_value != _Value; }
    //  bool operator<(T _Value){ return m_value < _Value; }
    //  bool operator>(T _Value){ return m_value > _Value; }
    //  bool operator<=(T _Value){ return m_value <= _Value; }
    //  bool operator>=(T _Value){ return m_value >= _Value; }
    bool has_value() const { return m_has_value; }
    T value() const { return m_value; }
    T value_or(T &&v) const { return m_has_value ? m_value : v; }
  };

} // namespace null_allow

template <class T> using optional = null_allow::null_allow<T>;
std::nullptr_t nullopt = nullptr;

using optional_int = optional<int>;

template <class T> class slice {
  using value_type = T;

private:
  optional<T> m_start;
  optional<T> m_stop;
  optional<T> m_step;

public:
  slice()
    : m_start()
    , m_stop()
    , m_step() {};
  template <class U>
  slice(U stop)
    : m_start()
    , m_stop(stop)
    , m_step() {};
  template <class U, class V>
  slice(U start, V stop)
    : m_start(start)
    , m_stop(stop)
    , m_step() {};
  template <class U, class V, class W>
  slice(U start, V stop, W step)
    : m_start(start)
    , m_stop(stop)
    , m_step(step) {};

  std::tuple<T, T, T, T> adjust_index(T length) const;
};

#ifndef PY_STR_UTIL
#define PY_STR_UTIL

namespace util {
  template <class _Elme> inline bool isRowBoundary(_Elme c)
  {
    switch (c) {
    case '\n':
    case '\r':
    case '\v':
    case '\f':
    case '\x1c':
    case '\x1d':
    case '\x1e':
    case '\x85':
      //    case '\u2028':
      //    case '\u2029':
      return true;
    default:
      return false;
    }
  }
  template <class _Elme> inline bool isprintable(_Elme c)
  {
    return (32 <= c) && (c <= 126);
  }

  template <class _Elme> inline bool isspace(_Elme c)
  {
    return ((9 <= c) && (c <= 13)) || ((28 <= c) && (c <= 32));
  }
  template <class _Elme> inline bool isascii(_Elme c)
  {
    return (0 <= c && c <= 127);
  }
  /*
template <class _Elme>
inline bool ishexdigit(_Elme c)
{
return ((48 <= c) && (c <= 57)) || ((65 <= c) && (c <= 70)) || ((97 <= c) && (c
<= 102));
}
template <class _Elme>
inline bool isoctdigit(_Elme c)
{
return (48 <= c && c <= 55)
}
template <class _Elme>
inline bool isbindigit(_Elme c)
{
return c == 48 || c == 49;
}
*/

  template <class T> inline std::string __to_string(T _Val, const char *_Format)
  {
    char buf[std::numeric_limits<T>::digits + 3];
    std::sprintf(buf, _Format, _Val);
    return buf;
  }
  inline std::string to_string_oct(short _Val)
  {
    return __to_string(_Val, "%o");
  }
  inline std::string to_string_oct(unsigned short _Val)
  {
    return __to_string(_Val, "%o");
  }
  inline std::string to_string_oct(int _Val) { return __to_string(_Val, "%o"); }
  inline std::string to_string_oct(unsigned int _Val)
  {
    return __to_string(_Val, "%o");
  }
  inline std::string to_string_oct(long _Val)
  {
    return __to_string(_Val, "%lo");
  }
  inline std::string to_string_oct(unsigned long _Val)
  {
    return __to_string(_Val, "%lo");
  }
  inline std::string to_string_oct(long long _Val)
  {
    return __to_string(_Val, "%llo");
  }
  inline std::string to_string_oct(unsigned long long _Val)
  {
    return __to_string(_Val, "%llo");
  }

  //
  inline std::string to_string_hex(short _Val)
  {
    return __to_string(_Val, "%x");
  }
  inline std::string to_string_hex(unsigned short _Val)
  {
    return __to_string(_Val, "%x");
  }
  inline std::string to_string_hex(int _Val) { return __to_string(_Val, "%x"); }
  inline std::string to_string_hex(unsigned int _Val)
  {
    return __to_string(_Val, "%x");
  }
  inline std::string to_string_hex(long _Val)
  {
    return __to_string(_Val, "%lx");
  }
  inline std::string to_string_hex(unsigned long _Val)
  {
    return __to_string(_Val, "%lx");
  }
  inline std::string to_string_hex(long long _Val)
  {
    return __to_string(_Val, "%llx");
  }
  inline std::string to_string_hex(unsigned long long _Val)
  {
    return __to_string(_Val, "%llx");
  }
  inline std::string to_string_HEX(short _Val)
  {
    return __to_string(_Val, "%X");
  }
  inline std::string to_string_HEX(unsigned short _Val)
  {
    return __to_string(_Val, "%X");
  }
  inline std::string to_string_HEX(int _Val) { return __to_string(_Val, "%X"); }
  inline std::string to_string_HEX(unsigned int _Val)
  {
    return __to_string(_Val, "%X");
  }
  inline std::string to_string_HEX(long _Val)
  {
    return __to_string(_Val, "%lX");
  }
  inline std::string to_string_HEX(unsigned long _Val)
  {
    return __to_string(_Val, "%lX");
  }
  inline std::string to_string_HEX(long long _Val)
  {
    return __to_string(_Val, "%llX");
  }
  inline std::string to_string_HEX(unsigned long long _Val)
  {
    return __to_string(_Val, "%llX");
  }

  template <class T> inline size_t decimal_place(T x)
  {
    size_t i = 0, j = 10;
    T t = x;
    while (t != std::floor(t)) {
      t = x * j;
      j *= 10;
      ++i;
    }
    return i;
  }

  template <class T> T sign(T n) { return (n > 0) - (n < 0); }

  template <class T>
  inline std::tuple<T, T, T, T> adjust_index(optional<T> _start,
                                             optional<T> _stop,
                                             optional<T> _step, T _length)
  {
    T start, stop, step = _step.value_or(1), upper, lower;
    T step_sign = sign(step);
    bool step_is_negative = step_sign < 0;
    /* Find lower and upper bounds for start and stop. */
    if (step_is_negative) {
      lower = -1;
      upper = _length + lower;
    } else {
      lower = 0;
      upper = _length;
    }
    // Compute start.
    if (_start.has_value()) {
      start = _start.value();
      if (sign(start) < 0) {
        start += _length;

        if (start < lower /* Py_LT */) {
          start = lower;
        }
      } else {
        if (start > upper /* Py_GT */) {
          start = upper;
        }
      }
    } else {
      start = step_is_negative ? upper : lower;
    }
    // Compute stop.
    if (_stop.has_value()) {
      stop = _stop.value();

      if (sign(stop) < 0) {
        stop += _length;
        if (stop < lower /* Py_LT */) {
          stop = lower;
        }
      } else {
        if (stop > upper /* Py_GT */) {
          stop = upper;
        }
      }
    } else {
      stop = step_is_negative ? lower : upper;
    }
    T len = 0;
    if (step < 0) {
      if (stop < start) {
        len = (start - stop - 1) / (-step) + 1;
      }
    } else {
      if (start < stop) {
        len = (stop - start - 1) / step + 1;
      }
    }
    return std::make_tuple(start, stop, step, len);
  }

  template <class T>
  inline std::tuple<T, T, T, T> adjust_index(optional<T> _start,
                                             optional<T> _stop, T _length)
  {
    return adjust_index<T>(_start, _stop, 1, _length);
  }
  inline void adjust_index(int &start, int &end, int len)
  {
    std::tie(start, end, std::ignore, std::ignore)
      = adjust_index<int>(start, end, 1, len);
  }
  template <class T> inline std::string itobin(T n)
  {
    std::string str;
    T p = 1;
    for (T i = sizeof(T) * 8; i--;)
      str.push_back(n & (p << i) ? '1' : '0');
    auto pos = str.find('1');
    if (pos != std::string::npos)
      return str.substr(pos);
    return "0";
  }
  inline void sepinsert(std::string &str, const std::string sep, size_t len)
  {
    std::string dst;
    size_t count = 0;
    auto start = str.rbegin();
    auto end = str.rend();
    for (; start != end; ++start) {
      ++count;
      dst.push_back(*start);
      if (!(count % len)) {
        dst += sep;
      }
    }
    if (dst.back() == sep.back())
      dst.pop_back();
    str.clear();
    std::reverse_copy(dst.begin(), dst.end(), std::back_inserter(str));
  }

  inline void __format_eq(std::string &str, char c, size_t len)
  {
    static std::regex mc("((-|\\+| )?(0[boxX])?)?([0-9a-fA-F,_.]+?)");
    std::smatch sub;
    std::regex_match(str, sub, mc);
    str = sub.str(1)
          + std::string((len - sub.str(1).size()) - sub.str(4).size(), c)
          + sub.str(4);
  }

  inline char _get_fill_char(std::string str)
  {
    return str.empty() ? ' ' : str[0];
  }

  inline size_t _get_size(std::string str)
  {
    return str.empty() ? 0 : std::stoul(str);
  }

} // namespace util
#endif

namespace format_parser {
  template <typename T>
  void _sign_format(T &str, std::string sign, bool negative);

  template <typename T,
            typename std::enable_if_t<!std::is_integral<T>::value
                                        && !std::is_floating_point<T>::value,
                                      std::nullptr_t> = nullptr>
  bool format(std::string r, T target, std::string &dst);

  template <typename T, typename std::enable_if_t<std::is_integral<T>::value,
                                                  std::nullptr_t> = nullptr>
  bool format(std::string r, T target, std::string &dst);

  template <typename T,
            typename std::enable_if_t<std::is_floating_point<T>::value,
                                      std::nullptr_t> = nullptr>
  bool format(std::string r, T target, std::string &dst);
} // namespace format_parser

template <class _Elme> class basic_string : public std::basic_string<_Elme> {
public:
  using transtable_t = std::unordered_map<_Elme, basic_string<_Elme>>;

private:
  size_t _back_index(int index) const { return this->size() + index; }
  basic_string<_Elme> _format_automatic(basic_string<_Elme> &_Str) const
  {
    return _Str;
  }
  template <class Head, class... Tail>
  basic_string<_Elme> _format_automatic(basic_string<_Elme> &_Str, Head head,
                                        Tail... tail) const
  {
    basic_string<_Elme> str;
    std::smatch sub;
    static std::regex match("\\{(![^:])?(:(.+?)?)?\\}");
    if (std::regex_search(_Str, sub, match)) {
      format_parser::format(sub.str(3), head, str);
      _Str = _Str.pyreplace(sub.str(0), str, 1);
      return this->_format_automatic(_Str, std::move(tail)...);
    }
    return _Str;
  }
  template <size_t N>
  basic_string<_Elme> _format(basic_string<_Elme> &_Str) const
  {
    return _Str;
  }
  template <size_t N, class Head, class... Tail>
  basic_string<_Elme> _format(basic_string<_Elme> &_Str, Head head,
                              Tail... tail) const // format_numbaring
  {
    basic_string<_Elme> str;
    std::smatch sub;
    std::regex num_match(std::string("\\{") + std::to_string(N)
                         + std::string("(![^:])?(:(.+?)?)?\\}"));
    while (std::regex_search(_Str, sub, num_match)) {
      format_parser::format(sub.str(4), head, str);
      _Str = _Str.pyreplace(sub.str(0), str);
    }
    return this->_format<N + 1>(_Str, std::move(tail)...);
  }
  template <class Map> basic_string<_Elme> _format_map(Map &map);
  template <class Map> static transtable_t _maketrans(Map &table_map)
  {
    transtable_t table;
    for (auto &&m : table_map) {
      table[m.first] = m.second;
    }
    return table;
  }

public:
  basic_string<_Elme>()
    : std::basic_string<_Elme>() {};
  basic_string<_Elme>(std::basic_string<_Elme> &&_Str)
    : std::basic_string<_Elme>(_Str) {};
  basic_string<_Elme>(const std::basic_string<_Elme> &_Str)
    : std::basic_string<_Elme>(_Str) {};
  using std::basic_string<_Elme>::basic_string;
  using std::basic_string<_Elme>::operator=;
  using std::basic_string<_Elme>::operator+=;

  // override//
  const _Elme &at(int _Index) const
  {
    return (_Index < 0) ? std::basic_string<_Elme>::at(_back_index(_Index))
                        : std::basic_string<_Elme>::at(_Index);
  }
  _Elme &at(int _Index)
  {
    return (_Index < 0) ? std::basic_string<_Elme>::at(_back_index(_Index))
                        : std::basic_string<_Elme>::at(_Index);
  }
  const _Elme &operator[](int _Index) const
  {
    return (_Index < 0) ? std::string::operator[](_back_index(_Index))
                        : std::string::operator[](_Index);
  }
  _Elme &operator[](int _Index)
  {
    return (_Index < 0) ? std::string::operator[](_back_index(_Index))
                        : std::string::operator[](_Index);
  }
  basic_string<_Elme>
  substr(const typename std::basic_string<_Elme>::size_type pos = 0,
         const typename std::basic_string<_Elme>::size_type n
         = std::basic_string<_Elme>::npos) const noexcept
  {
    return basic_string<_Elme>(*this, pos, n,
                               std::basic_string<_Elme>::get_allocator());
  }

  /////

  basic_string<_Elme> operator*(size_t i) const;
  basic_string<_Elme> &operator*=(size_t i);
  basic_string<_Elme> operator[](std::initializer_list<optional_int> slice);
  basic_string<_Elme> capitalize(void) const noexcept;
  basic_string<_Elme> center(size_t width, _Elme fillchar = ' ') const;
  size_t count(basic_string<_Elme> sub, int start = 0,
               int end = std::numeric_limits<int>::max()) const;
  bool endswith(basic_string<_Elme> suffix, int start = 0,
                int end = std::numeric_limits<int>::max()) const;
  basic_string<_Elme> expandtabs(size_t tabsize = 8) const;
  int pyfind(basic_string<_Elme> sub, int start = 0,
             int end = std::numeric_limits<int>::max()) const;
  template <class... Args> basic_string<_Elme> format(Args... args);
  basic_string<_Elme>
  format_map(std::map<basic_string<_Elme>, basic_string<_Elme>> map);
  basic_string<_Elme>
  format_map(std::unordered_map<basic_string<_Elme>, basic_string<_Elme>> map);
  int index(basic_string<_Elme> sub, int start = 0,
            int end = std::numeric_limits<int>::max()) const;
  bool isalnum(void) const;
  bool isalpha(void) const;
  bool isascii(void) const;
  bool isdecimal(void) const;
  bool isdigit(void) const;
  bool islower(void) const;
  bool isnumeric(void) const;
  bool isprintable(void) const;
  bool isspace(void) const;
  bool istitle(void) const;
  bool isupper(void) const;
  template <typename _Iterable>
  basic_string<_Elme> join(_Iterable &&iterable) const;
  basic_string<_Elme> ljust(size_t width, _Elme fillchar = ' ') const;
  basic_string<_Elme> lower(void) const noexcept;
  basic_string<_Elme> lstrip(void) const;
  basic_string<_Elme> lstrip(basic_string<_Elme> chars) const;
  void partition(basic_string<_Elme> sep, basic_string<_Elme> &dst1,
                 basic_string<_Elme> &dst2, basic_string<_Elme> &dst3) const;
  template <typename _Iterable>
  void partition(basic_string<_Elme> sep, _Iterable &iterable) const;
  basic_string<_Elme>
  pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new,
            size_t count = std::numeric_limits<size_t>::max()) const;
  int pyrfind(basic_string<_Elme> sub, int start = 0,
              int end = std::numeric_limits<int>::max()) const;
  int rindex(basic_string<_Elme> sub, int start = 0,
             int end = std::numeric_limits<int>::max()) const;
  basic_string<_Elme> rjust(size_t width, _Elme fillchar = ' ') const;
  void rpartition(basic_string<_Elme> sep, basic_string<_Elme> &dst1,
                  basic_string<_Elme> &dst2, basic_string<_Elme> &dst3) const;
  template <typename _Iterable>
  void rpartition(basic_string<_Elme> sep, _Iterable &iterable) const;

  template <typename _Iterable>
  void rsplit(_Iterable &dst, int maxsplit = -1) const;
  template <typename _Iterable>
  void rsplit(_Iterable &dst, basic_string<_Elme> sep, int maxsplit = -1) const;

  basic_string<_Elme> rstrip(void) const;
  basic_string<_Elme> rstrip(basic_string<_Elme> chars) const;

  template <typename _Iterable>
  void split(_Iterable &dst, int maxsplit = -1) const;
  template <typename _Iterable>
  void split(_Iterable &dst, basic_string<_Elme> sep, int maxsplit = -1) const;

  template <typename _Iterable>
  void splitlines(_Iterable &dst, bool keepends = false) const;
  bool startswith(basic_string<_Elme> suffix, int start = 0,
                  int end = std::numeric_limits<int>::max()) const;
  basic_string<_Elme> strip(void) const;
  basic_string<_Elme> strip(basic_string<_Elme> chars) const;
  basic_string<_Elme> swapcase(void) const;
  basic_string<_Elme> title(void) const;
  basic_string<_Elme> translate(transtable_t &table) const;
  basic_string<_Elme> upper(void) const;
  basic_string<_Elme> zfill(size_t width) const;
  basic_string<_Elme> slice(optional_int index) const;
  basic_string<_Elme> slice(optional_int start, optional_int end) const;
  basic_string<_Elme> slice(optional_int start, optional_int end,
                            optional_int step) const;
  static transtable_t
  maketrans(std::unordered_map<_Elme, basic_string<_Elme>> table_map)
  {
    return basic_string<_Elme>::_maketrans(table_map);
  }
  static transtable_t maketrans(std::map<_Elme, basic_string<_Elme>> table_map)
  {
    return basic_string<_Elme>::_maketrans(table_map);
  }
  static transtable_t maketrans(basic_string<_Elme> from,
                                basic_string<_Elme> to)
  {
    return basic_string<_Elme>::maketrans(from, to, "");
  }
  static transtable_t maketrans(basic_string<_Elme> from,
                                basic_string<_Elme> to,
                                basic_string<_Elme> delchars)
  {
    transtable_t table;
    for (int i = std::min(from.size(), to.size()); i--;) {
      table[from[i]] = to[i];
    }
    for (auto &&s : delchars) {
      table[s] = "";
    }
    return table;
  }
};

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

template <class _Elme>
basic_string<_Elme> operator+(basic_string<_Elme> r, basic_string<_Elme> l)
{
  r += l;
  return r;
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::operator*(size_t i) const
{
  if (i <= 0)
    return "";
  basic_string<_Elme> str(*this);
  str.reserve(str.size() * i);
  for (; --i;) {
    str += *this;
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> &basic_string<_Elme>::operator*=(size_t i)
{
  *this = *this * i;
  return *this;
}
template <class _Elme>
basic_string<_Elme>
basic_string<_Elme>::operator[](std::initializer_list<optional_int> slice)
{
  basic_string<_Elme> str;
  auto in = slice.begin();
  optional_int index1 = *in++;
  optional_int index2 = *in++;
  optional_int index3 = *in;

  switch (slice.size()) {
  case 0:
    return *this;
  case 1:
    return this->slice(index1);
  case 2:
    return this->slice(index1, index2);
  default: // case 3:
    return this->slice(index1, index2, index3);
  }
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::capitalize(void) const noexcept
{
  basic_string<_Elme> str = this->lower();
  str[0] = std::toupper(str[0]);
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::center(size_t width,
                                                _Elme fillchar) const
{
  size_t r, l, len = this->size();
  if (width <= len)
    return *this;
  l = width - len;
  r = l / 2 + l % 2;

  return basic_string<_Elme>(l - r, fillchar) + *this
         + basic_string<_Elme>(r, fillchar);
}
template <class _Elme>
size_t basic_string<_Elme>::count(basic_string<_Elme> sub, int start,
                                  int end) const
{
  size_t sublen = sub.size();
  if (sublen == 0) {
    return this->size() + 1;
  }
  util::adjust_index(start, end, this->size());
  size_t nummatches = 0;
  size_t cursor = this->find(sub, start);
  while ((cursor != std::basic_string<_Elme>::npos)
         && (cursor + sublen <= end)) {
    cursor = this->find(sub, cursor + sublen);
    ++nummatches;
  }
  return nummatches;
}
template <class _Elme>
bool basic_string<_Elme>::endswith(basic_string<_Elme> suffix, int start,
                                   int end) const
{
  int len = this->size();
  int sublen = suffix.size();

  util::adjust_index(start, end, len);
  if (end - start < sublen || start > len) {
    return false;
  }
  if (end - sublen > start) {
    start = end - sublen;
  }
  if (end - start >= sublen) {
    return (!std::memcmp(this->c_str() + start * sizeof(_Elme), suffix.c_str(),
                         sublen * sizeof(_Elme)));
  }
  return false;
}
template <class _Elme>
inline basic_string<_Elme> basic_string<_Elme>::expandtabs(size_t tabsize) const
{
  return this->pyreplace("\t", basic_string<_Elme>(tabsize, ' '));
}
template <class _Elme>
int basic_string<_Elme>::pyfind(basic_string<_Elme> sub, int start,
                                int end) const
{
  util::adjust_index(start, end, this->size());
  size_t result = this->find(sub, start);
  if (result == std::basic_string<_Elme>::npos || (result + sub.size() > end)) {
    return -1;
  }
  return result;
}
template <class _Elme>
template <class... Args>
basic_string<_Elme> basic_string<_Elme>::format(Args... args)
{
  basic_string<_Elme> str(*this);
  static std::regex match("\\{(![^:])?(:(.+?)?)?\\}");
  if (std::regex_search(str, match)) {
    return this->_format_automatic(str, std::move(args)...);
  }
  return this->_format<0>(str, std::move(args)...);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::format_map(
  std::map<basic_string<_Elme>, basic_string<_Elme>> map)
{
  return this->_format_map(map);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::format_map(
  std::unordered_map<basic_string<_Elme>, basic_string<_Elme>> map)
{
  return this->_format_map(map);
}
template <class _Elme>
template <class Map>
inline basic_string<_Elme> basic_string<_Elme>::_format_map(Map &map)
{
  basic_string<_Elme> str(*this);
  basic_string<_Elme> key, val;
  size_t s_cursor = str.find("{");
  size_t e_cursor = str.find("}", s_cursor);
  size_t key_len;
  while (s_cursor != std::basic_string<_Elme>::npos
         && e_cursor != std::basic_string<_Elme>::npos) {
    key_len = e_cursor - s_cursor - 1;
    key = str.substr(s_cursor + 1, key_len);

    if (map.count(key)) {
      val = map[key];
      str.replace(s_cursor, key_len + 2, val);
      e_cursor -= key_len + 2;
      e_cursor += val.size();
    }
    s_cursor = str.find("{", e_cursor);
    e_cursor = str.find("}", s_cursor);
  }
  return str;
}
template <class _Elme>
inline int basic_string<_Elme>::index(basic_string<_Elme> sub, int start,
                                      int end) const
{
  return this->pyfind(sub, start, end);
}
template <class _Elme> bool basic_string<_Elme>::isalnum(void) const
{
  for (auto &&s : *this) {
    if (!std::isalnum(s))
      return false;
  }
  return !this->empty();
}
template <class _Elme> bool basic_string<_Elme>::isalpha(void) const
{
  for (auto &&s : *this) {
    if (!std::isalpha(s))
      return false;
  }
  return !this->empty();
}
template <class _Elme> bool basic_string<_Elme>::isascii(void) const
{
  if (this->empty())
    return true;
  for (auto &&s : *this) {
    if (!util::isascii(s))
      return false;
  }
  return true;
}
template <class _Elme> bool basic_string<_Elme>::isdecimal(void) const
{
  for (auto &&s : *this) {
    if (!std::isdigit(s))
      return false;
  }
  return !this->empty();
}
template <class _Elme> bool basic_string<_Elme>::isdigit(void) const
{
  return this->isdecimal();
}
template <class _Elme> bool basic_string<_Elme>::islower(void) const
{
  for (auto &&s : *this) {
    if (!std::islower(s))
      return false;
  }
  return !this->empty();
}
template <class _Elme> bool basic_string<_Elme>::isnumeric(void) const
{
  return this->isdecimal();
}
template <class _Elme> bool basic_string<_Elme>::isprintable(void) const
{
  for (auto &&s : *this) {
    if (!util::isprintable<_Elme>(s))
      return false;
  }
  return !this->empty();
}
template <class _Elme> bool basic_string<_Elme>::isspace(void) const
{
  for (auto &&s : *this) {
    if (!util::isspace<_Elme>(s))
      return false;
  }
  return !this->empty();
}
template <class _Elme> bool basic_string<_Elme>::istitle(void) const
{
  if (this->size() == 0)
    return false;
  if (this->size() == 1)
    return std::isupper(this->at(0));
  bool cased = false, previous_is_cased = false;
  for (auto &&s : *this) {
    if (std::isupper(s)) {
      if (previous_is_cased) {
        return false;
      }
      previous_is_cased = true;
      cased = true;
    } else if (std::islower(s)) {
      if (!previous_is_cased) {
        return false;
      }
      previous_is_cased = true;
      cased = true;
    } else {
      previous_is_cased = false;
    }
  }
  return cased;
}
template <class _Elme> bool basic_string<_Elme>::isupper(void) const
{
  {
    for (auto &&s : *this) {
      if (!std::isupper(s))
        return false;
    }
    return !this->empty();
  }
}

template <class _Elme>
template <typename _Iterable>
basic_string<_Elme> basic_string<_Elme>::join(_Iterable &&iterable) const
{
  basic_string<_Elme> str = "";
  const basic_string<_Elme> *sep = &str;
  for (auto &&i : iterable) {
    str += *sep;
    str += i;
    sep = this;
  }
  return str;
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::ljust(size_t width,
                                               _Elme fillchar) const
{
  if (width <= this->size()) {
    return *this;
  }
  return *this + basic_string<_Elme>(width - this->size(), fillchar);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::lower(void) const noexcept
{
  {
    basic_string<_Elme> str(*this);
    for (auto &&s : str) {
      s = std::tolower(s);
    }
    return str;
  }
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::lstrip(void) const
{
  if (this->empty())
    return *this;
  auto itr = this->begin();
  while (util::isspace(*itr))
    ++itr;
  return this->substr(std::distance(this->begin(), itr));
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::lstrip(basic_string<_Elme> chars) const
{
  if (this->empty())
    return *this;
  if (chars.empty())
    return this->lstrip();
  auto itr = this->begin();
  while (chars.find(*itr) != std::basic_string<_Elme>::npos)
    ++itr;
  return this->substr(std::distance(this->begin(), itr));
}
template <class _Elme>
void basic_string<_Elme>::partition(basic_string<_Elme> sep,
                                    basic_string<_Elme> &dst1,
                                    basic_string<_Elme> &dst2,
                                    basic_string<_Elme> &dst3) const
{
  size_t index = this->find(sep);
  if (index == std::basic_string<_Elme>::npos) {
    dst1 = *this;
    dst2 = "";
    dst3 = "";
  } else {
    dst1 = this->substr(0, index);
    dst2 = sep;
    dst3 = this->substr(index + sep.size());
  }
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::partition(basic_string<_Elme> sep,
                                    _Iterable &iterable) const
{
  basic_string<_Elme> dst1, dst2, dst3;
  this->partition(sep, dst1, dst2, dst3);
  iterable = { dst1, dst2, dst3 };
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::pyreplace(basic_string<_Elme> old,
                                                   basic_string<_Elme> _new,
                                                   size_t count) const
{
  size_t cursor = 0;
  basic_string<_Elme> s(*this);
  size_t oldlen = old.size(), newlen = _new.size();
  cursor = s.find(old, cursor);
  while (cursor != std::basic_string<_Elme>::npos && cursor <= s.size()
         && count > 0) {
    s.replace(cursor, oldlen, _new);
    cursor += newlen;
    if (oldlen != 0) {
      cursor = s.find(old, cursor);
    } else {
      ++cursor;
    }
    --count;
  }
  return s;
}
template <class _Elme>
int basic_string<_Elme>::pyrfind(basic_string<_Elme> sub, int start,
                                 int end) const
{
  util::adjust_index(start, end, this->size());
  int result = this->rfind(sub, end - sub.size());
  if (result == std::string::npos || result < start
      || (result + sub.size() > end)) {
    return -1;
  }
  return result;
}
template <class _Elme>
inline int basic_string<_Elme>::rindex(basic_string<_Elme> sub, int start,
                                       int end) const
{
  return this->pyrfind(sub, start, end);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::rjust(size_t width,
                                               _Elme fillchar) const
{
  if (width <= this->size()) {
    return *this;
  }
  return basic_string<_Elme>(width - this->size(), fillchar) + *this;
}

template <class _Elme>
void basic_string<_Elme>::rpartition(basic_string<_Elme> sep,
                                     basic_string<_Elme> &dst1,
                                     basic_string<_Elme> &dst2,
                                     basic_string<_Elme> &dst3) const
{
  int index = this->pyrfind(sep);
  if (index == -1) {
    dst1 = "";
    dst2 = "";
    dst3 = *this;
  } else {
    dst1 = this->substr(0, index);
    dst2 = sep;
    dst3 = this->substr(index + sep.size());
  }
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::rpartition(basic_string<_Elme> sep,
                                     _Iterable &iterable) const
{
  basic_string<_Elme> dst1, dst2, dst3;
  this->rpartition(sep, dst1, dst2, dst3);
  iterable = { dst1, dst2, dst3 };
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::rsplit(_Iterable &result, int maxsplit) const
{
  if (result.size() != 0)
    result.clear();
  auto start = this->rbegin();
  auto end = this->rend();
  int index = this->size() - 1, len = 0;
  basic_string<_Elme> tmp;
  if (maxsplit < 0) {
    maxsplit = std::numeric_limits<int>::max();
  }
  for (; start != end && maxsplit; ++start) {
    if (util::isspace(*start)) {
      if (len != 0) {
        result.insert(result.begin(), this->substr(index, len));
        --maxsplit;
        index -= len;
      }
      --index;
      len = 0;
    } else {
      ++len;
    }
  }
  tmp = this->substr(0, ++index);
  tmp = tmp.rstrip();
  if (tmp.size() != 0) {
    result.insert(result.begin(), tmp);
  }
}

template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::rsplit(_Iterable &result, basic_string<_Elme> sep,
                                 int maxsplit) const
{
  if (sep.empty())
    return this->rsplit(result, maxsplit);
  if (result.size() != 0)
    result.clear();
  size_t index, pre_index = std::basic_string<_Elme>::npos,
                sep_len = sep.size();
  if (maxsplit < 0) {
    maxsplit = std::numeric_limits<int>::max();
  }
  for (; maxsplit--;) {
    index = this->rfind(sep, pre_index);
    if (index == std::basic_string<_Elme>::npos)
      break;
    index += sep_len;
    result.insert(result.begin(), this->substr(index, ++pre_index - index));
    index -= sep_len;
    pre_index = --index;
  }
  result.insert(result.begin(), this->substr(0, ++pre_index));
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::rstrip(void) const
{
  if (this->empty())
    return *this;
  basic_string<_Elme> str(*this);
  while (util::isspace(str.back()))
    str.pop_back();
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::rstrip(basic_string<_Elme> chars) const
{
  if (this->empty())
    return *this;
  if (chars.empty())
    return this->rstrip();
  basic_string<_Elme> str(*this);
  while (chars.find(str.back()) != std::basic_string<_Elme>::npos)
    str.pop_back();
  return str;
}

template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::split(_Iterable &result, int maxsplit) const
{
  if (result.size() != 0)
    result.clear();
  auto start = this->begin();
  auto end = this->end();
  int index = 0, len = 0;
  basic_string<_Elme> tmp;
  if (maxsplit < 0) {
    maxsplit = std::numeric_limits<int>::max();
  }
  for (; start != end && maxsplit; ++start) {
    if (util::isspace(*start)) {
      if (len != 0) {
        result.push_back(this->substr(index, len));
        --maxsplit;
        index += len;
      }
      ++index;
      len = 0;
    } else {
      ++len;
    }
  }
  tmp = this->substr(index);
  tmp = tmp.lstrip();
  if (tmp.size() != 0) {
    result.push_back(tmp);
  }
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::split(_Iterable &result, basic_string<_Elme> sep,
                                int maxsplit) const
{
  if (sep.empty())
    return this->split(result, maxsplit);
  if (result.size() != 0)
    result.clear();
  size_t index, pre_index = 0, sep_len = sep.size();
  if (maxsplit < 0) {
    maxsplit = std::numeric_limits<int>::max();
  }
  for (; maxsplit--;) {
    index = this->find(sep, pre_index);
    if (index == std::basic_string<_Elme>::npos)
      break;
    result.push_back(this->substr(pre_index, index - pre_index));
    pre_index = index + sep_len;
  }
  result.push_back(this->substr(pre_index));
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::splitlines(_Iterable &dst, bool keepends) const
{
  if (!dst.empty()) {
    dst.clear();
  }
  size_t len = this->size(), i, j, eol;
  for (i = j = 0; i < len;) {
    while (i < len && !util::isRowBoundary<_Elme>(this->at(i)))
      ++i;
    eol = i;
    if (i < len) {
      if (this->at(i) == '\r' && i + 1 < len && this->at(i + 1) == '\n') {
        i += 2;
      } else {
        ++i;
      }
      if (keepends)
        eol = i;
    }
    dst.push_back(this->substr(j, eol - j));
    j = i;
  }
  if (j < len) {
    dst.push_back(this->substr(j, len - j));
  }
}
template <class _Elme>
bool basic_string<_Elme>::startswith(basic_string<_Elme> suffix, int start,
                                     int end) const
{
  int len = this->size();
  int sublen = suffix.size();
  const _Elme *self = this->c_str();
  const _Elme *sub = suffix.c_str();

  util::adjust_index(start, end, len);
  if (start + sublen > len) {
    return false;
  }
  if (end - start >= sublen) {
    return (
      !std::memcmp(self + start * sizeof(_Elme), sub, sublen * sizeof(_Elme)));
  }
  return false;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::strip(void) const
{
  if (this->empty())
    return *this;
  basic_string<_Elme> str(*this);
  while (util::isspace(str.back()))
    str.pop_back();
  return str.lstrip();
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::strip(basic_string<_Elme> chars) const
{
  if (this->empty())
    return *this;
  basic_string<_Elme> str(*this);
  while (chars.find(str.back()) != std::basic_string<_Elme>::npos)
    str.pop_back();
  return str.lstrip(chars);
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::swapcase(void) const
{
  basic_string<_Elme> str(*this);
  for (auto &&s : str) {
    if (std::islower(s)) {
      s = std::toupper(s);
    } else if (std::isupper(s)) {
      s = std::tolower(s);
    }
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::title(void) const
{
  basic_string<_Elme> str(*this);
  bool previous_is_cased = false;
  for (auto &&s : str) {
    if (std::islower(s)) {
      if (!previous_is_cased) {
        s = std::toupper(s);
      }
      previous_is_cased = true;
    } else if (std::isupper(s)) {
      if (previous_is_cased) {
        s = std::tolower(s);
      }
      previous_is_cased = true;
    } else {
      previous_is_cased = false;
    }
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::translate(transtable_t &table) const
{
  basic_string<_Elme> str;
  for (auto &&s : *this) {
    if (table.count(s)) {
      str += table[s];
    } else {
      str.push_back(s);
    }
  }
  return str;
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::upper(void) const
{
  basic_string<_Elme> str(*this);
  for (auto &&s : str) {
    s = std::toupper(s);
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::zfill(size_t width) const
{
  int len = this->size();
  if (len >= width)
    return *this;
  if (this->empty())
    return basic_string<_Elme>(width, '0');
  basic_string<_Elme> str(*this);
  int fill = width - len;
  str = basic_string<_Elme>(fill, '0') + str;

  if (str.at(fill) == '+' || str.at(fill) == '-') {
    str.at(0) = str.at(fill);
    str.at(fill) = '0';
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::slice(optional_int index) const
{
  if (!index.has_value()) {
    return *this;
  }
  return basic_string<_Elme>(1, this->at(index.value()));
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::slice(optional_int start,
                                               optional_int end) const
{
  int s, len;
  std::tie(s, std::ignore, std::ignore, len)
    = util::adjust_index<int>(start, end, this->size());
  return this->substr(s, len);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::slice(optional_int start,
                                               optional_int end,
                                               optional_int step) const
{
  if (step == 0)
    return "";
  if (!step.has_value() || step == 1)
    return this->slice(start, end);

  int start_i, step_i, len_i;
  basic_string<_Elme> str = "";
  std::tie(start_i, std::ignore, step_i, len_i)
    = util::adjust_index<int>(start, end, step, this->size());

  for (int i = 0; i < len_i; i += 1) {
    str.push_back(this->at(start_i));
    start_i += step_i;
  }
  return str;
}

namespace format_parser {
  static std::regex format_spce_regex(
    "((.?)(<|>|=|\\^))?(\\+|-| "
    ")?(#)?(0)?(\\d+?)?(,|_)?([.]\\d+?)?([bcdeEfFgGnosxX%])?");
  template <typename T = string>
  void _sign_format(T &str, std::string sign, bool negative)
  {
    if (sign == "+") {
      str.insert(0, (negative ? "-" : "+"));
    } else if (sign == " ") {
      str.insert(0, (negative ? "-" : " "));
    } else { // sub.str(4) == "-"
      str.insert(0, (negative ? "-" : ""));
    }
  }

  template <typename T,
            typename std::enable_if_t<!std::is_integral<T>::value
                                        && !std::is_floating_point<T>::value,
                                      std::nullptr_t>>
  bool format(std::string r, T target, std::string &dst)
  {
    std::smatch sub;
    std::stringstream stm;
    stm << target;
    string str = stm.str();
    bool result = std::regex_match(r, sub, format_spce_regex);
    if (result) {
      char fill_char = util::_get_fill_char(sub.str(2));
      size_t pad_size = util::_get_size(sub.str(7));

      if (sub.str(3) == "^") {
        dst = str.center(pad_size, fill_char);
      } else if (sub.str(3) == ">") {
        dst = str.rjust(pad_size, fill_char);
      } else {
        dst = str.ljust(pad_size, fill_char);
      }
    }
    return result;
  }
  template <typename T, typename std::enable_if_t<std::is_integral<T>::value,
                                                  std::nullptr_t>>
  bool format(std::string r, T target, std::string &dst)
  {
    std::smatch sub;
    string str;
    bool result = std::regex_match(r, sub, format_spce_regex);
    if (result) {
      bool negative = target < 0;
      target = negative ? -target : target;
      char fill_char = util::_get_fill_char(sub.str(2));
      size_t pad_size = util::_get_size(sub.str(7));
      if (sub.str(10) == "b") {
        str = util::itobin(target);
        util::sepinsert(str, sub.str(8), 4);
      } else if (sub.str(10) == "c") {
        str = static_cast<char>(target);
      } else if (sub.str(10) == "o") {
        str = util::to_string_oct(target);
        util::sepinsert(str, sub.str(8), 4);
      } else if (sub.str(10) == "x") {
        str = util::to_string_hex(target);
        util::sepinsert(str, sub.str(8), 4);
      } else if (sub.str(10) == "X") {
        str = util::to_string_HEX(target);
        util::sepinsert(str, sub.str(8), 4);
      } else if (!sub.str(10).empty()
                 && std::string("eEfFgGn%").find(sub.str(10))
                      != std::string::npos) {
        return negative ? format(r, static_cast<float>(-target), dst)
                        : format(r, static_cast<float>(target), dst);
      } else // sub.str(10) == "d"
      {
        str = std::to_string(target);
        util::sepinsert(str, sub.str(8), 3);
      }

      if (!sub.str(5).empty()) {
        if (sub.str(10) == "b") {
          str.insert(0, "0b");
        } else if (sub.str(10) == "o") {
          str.insert(0, "0o");
        } else if (sub.str(10) == "x") {
          str.insert(0, "0x");
        } else if (sub.str(10) == "X") {
          str.insert(0, "0X");
        }
      }

      _sign_format(str, sub.str(4), negative);

      if (!sub.str(6).empty()) {
        fill_char = '0';
        util::__format_eq(str, fill_char, pad_size);
        dst = str;
      } else if (sub.str(3) == "=") {
        util::__format_eq(str, fill_char, pad_size);
        dst = str;
      } else if (sub.str(3) == "<") {
        dst = str.ljust(pad_size, fill_char);
      } else if (sub.str(3) == "^") {
        dst = str.center(pad_size, fill_char);
      } else {
        dst = str.rjust(pad_size, fill_char);
      }
    }
    return result;
  }
  template <typename T, typename std::enable_if_t<
                          std::is_floating_point<T>::value, std::nullptr_t>>
  bool format(std::string r, T target, std::string &dst)
  {
    std::smatch sub;
    std::stringstream stm;
    string str;
    bool result = std::regex_match(r, sub, format_spce_regex);
    if (result) {
      bool negative = target < 0;
      target = negative ? -target : target;
      char fill_char = util::_get_fill_char(sub.str(2));
      size_t pad_size = util::_get_size(sub.str(7));
      size_t dp = 6;
      if (!sub.str(9).empty()) {
        dp = std::stoul(sub.str(9).substr(1));
      }

      if (sub.str(10) == "e") {
        stm << std::scientific << std::setprecision(dp) << target;
        str = stm.str();
      } else if (sub.str(10) == "E") {
        stm << std::scientific << std::uppercase << std::setprecision(dp)
            << target;
        str = stm.str();
      } else if (sub.str(10) == "f" || sub.str(10) == "F") {
        stm << std::fixed << std::setprecision(dp) << target;
        str = stm.str();
      } else if (!sub.str(10).empty()
                 && (sub.str(10) == "g" || sub.str(10) == "n")) {
        stm << std::fixed << std::setprecision(dp) << target;
        str = stm.str();
      } else if (sub.str(10) == "G") {
        stm << std::scientific << std::uppercase << std::setprecision(dp)
            << target;
        str = stm.str();
      } else if (sub.str(10) == "%") {
        stm << std::fixed << std::setprecision(dp) << target * 100 << "%";
        str = stm.str();
      } else {
        dp = util::decimal_place(target);
        dp = dp == 0 ? 1 : dp;
        stm << std::fixed << std::setprecision(dp) << target;
        str = stm.str();
      }

      _sign_format(str, sub.str(4), negative);

      if (!sub.str(6).empty()) {
        fill_char = '0';
        util::__format_eq(str, fill_char, pad_size);
        dst = str;
      } else if (sub.str(3) == "=") {
        util::__format_eq(str, fill_char, pad_size);
        dst = str;
      } else if (sub.str(3) == "<") {
        dst = str.ljust(pad_size, fill_char);
      } else if (sub.str(3) == "^") {
        dst = str.center(pad_size, fill_char);
      } else {
        dst = str.rjust(pad_size, fill_char);
      }
    }
    return result;
  }
} // namespace format_parser

} // namespace py

#define PYS_DEBUG
#ifdef PYS_DEBUG

template <class T>
std::ostream &operator<<(std::ostream &dst, const py::optional<T> &i)
{
  if (i.has_value()) {
    return dst << "optional(" << i.value() << ")";
  }
  return dst << "nullopt";
}

#endif

using py_string = py::string;
#endif // include guard PY_STRING_HPP
