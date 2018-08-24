#pragma once
#ifndef PY_STRING_HPP
#define PY_STRING_HPP

#include <string>
#include <cstring>
#include <cctype>
#include <cwctype>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <initializer_list>
#include <sstream>

namespace py
{

namespace null_allow
{
template <class T>
class null_allow
{
private:
  T value_m;
  bool null_m;

public:
  null_allow<T>(nullptr_t _Null) : null_m(true) {}
  null_allow<T>(T _Value) : value_m(_Value), null_m(false) {}
  null_allow<T> &operator=(nullptr_t _Null)
  {
    null_m = true;
    return *this;
  }
  null_allow<T> &operator=(T _Value)
  {
    value_m = _Value;
    return *this;
  }
  bool operator==(nullptr_t _Null) { return null_m; }
  bool operator!=(nullptr_t _Null) { return !null_m; }
  bool operator==(T _Value) { return value_m == _Value; }
  bool operator!=(T _Value) { return value_m != _Value; }
  operator T() const { return value_m; }
  operator nullptr_t() const { return nullptr; }
};

} // namespace null_allow
using null_int_t = null_allow::null_allow<int>;

#ifndef PY_STR_UTIL
#define PY_STR_UTIL

namespace util
{
template <class _Elme>
inline bool isRowBoundary(_Elme c)
{
  switch (c)
  {
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
template <class _Elme>
inline bool isprintable(_Elme c)
{
  return (32 <= c) && (c <= 126);
}

template <class _Elme>
inline bool isspace(_Elme c)
{
  return ((9 <= c) && (c <= 13)) || ((28 <= c) && (c <= 32));
}
inline void adjust_index(int &start, int &end, int len)
{
  if (end > len)
  {
    end = len;
  }
  else if (end < 0)
  {
    end += len;
    if (end < 0)
      end = 0;
  }
  if (start < 0)
  {
    start += len;
    if (start < 0)
      start = 0;
  }
}

inline void adjust_index(null_int_t &start, null_int_t &end, int len)
{
  start = start == nullptr ? 0 : start;
  start = start < 0 ? len - start : start;

  end = (end == nullptr) || (end > len) ? len : end;
  end = (end < 0) ? len - end : end;
}

} // namespace util
#endif

template <class _Elme>
class basic_string : public std::basic_string<_Elme>
{
public:
  using transtable_t = std::unordered_map<_Elme, basic_string<_Elme>>;

private:
  size_t _back_index(int index) const { return this->size() + index; }
  void pop_front(void)
  {
    if (!empty())
      this->erase(0, 1);
  }
  basic_string<_Elme> _format(basic_string<_Elme> &_Str) const { return _Str; }
  template <class Head, class... Tail>
  basic_string<_Elme> _format(basic_string<_Elme> &_Str, Head head, Tail... tail) const
  {
    std::basic_stringstream<_Elme> stm;
    stm << head;
    int index = _Str.pyfind("{}");
    if (index == -1)
    {
      return _Str;
    }
    _Str = _Str.pyreplace("{}", stm.str(), 1);
    return this->_format(_Str, std::move(tail)...);
  }
  template <class Map>
  basic_string<_Elme> _format_map(Map &map);
  template <class Map>
  static transtable_t _maketrans(Map &table_map)
  {
    transtable_t table;
    for (auto m : table_map)
    {
      if (m.first.size() != 0)
      {
        basic_string<_Elme> key = m.first;
        table[key[0]] = m.second;
      }
    }
    return table;
  }

public:
  using std::basic_string<_Elme>::basic_string;
  using std::basic_string<_Elme>::operator+=;
  using std::basic_string<_Elme>::operator=;
  basic_string<_Elme>() : std::basic_string<_Elme>(){};
  basic_string<_Elme>(std::basic_string<_Elme> _Str) : std::basic_string<_Elme>(_Str){};
  basic_string<_Elme>(_Elme _Chr) : std::basic_string<_Elme>(1, _Chr){};
  //override//
  const _Elme &at(int _Index) const { return (_Index < 0) ? std::basic_string<_Elme>::at(_back_index(_Index)) : std::basic_string<_Elme>::at(_Index); }
  _Elme &at(int _Index) { return (_Index < 0) ? std::basic_string<_Elme>::at(_back_index(_Index)) : std::basic_string<_Elme>::at(_Index); }
  const _Elme &operator[](int _Index) const { return this->at(_Index); }
  _Elme &operator[](int _Index) { return this->at(_Index); }
  /////

  basic_string<_Elme> operator*(size_t i);
  basic_string<_Elme> &operator*=(size_t i);
  basic_string<_Elme> operator[](std::initializer_list<null_allow::null_allow<int>> slice);
  basic_string<_Elme> capitalize(void) const;
  basic_string<_Elme> center(size_t width, _Elme fillchar = ' ') const;
  size_t count(basic_string<_Elme> sub) const;
  size_t count(basic_string<_Elme> sub, int start) const;
  size_t count(basic_string<_Elme> sub, int start, int end) const;
  bool endswith(basic_string<_Elme> suffix) const;
  bool endswith(basic_string<_Elme> suffix, int start) const;
  bool endswith(basic_string<_Elme> suffix, int start, int end) const;
  basic_string<_Elme> expandtabs(size_t tabsize = 8) const;
  int pyfind(basic_string<_Elme> sub) const;
  int pyfind(basic_string<_Elme> sub, int start) const;
  int pyfind(basic_string<_Elme> sub, int start, int end) const;
  template <class... Args>
  basic_string<_Elme> format(Args... args);
  basic_string<_Elme> format_map(std::map<basic_string<_Elme>, basic_string<_Elme>> map);
  basic_string<_Elme> format_map(std::unordered_map<basic_string<_Elme>, basic_string<_Elme>> map);
  int index(basic_string<_Elme> sub) const;
  int index(basic_string<_Elme> sub, int start) const;
  int index(basic_string<_Elme> sub, int start, int end) const;
  bool isalnum(void) const;
  bool isalpha(void) const;
  bool isdecimal(void) const;
  bool isdigit(void) const;
  bool islower(void) const;
  bool isnumeric(void) const;
  bool isprintable(void) const;
  bool isspace(void) const;
  bool istitle(void) const;
  bool isupper(void) const;
  template <typename _Iterable>
  basic_string<_Elme> join(_Iterable iterable) const;
  basic_string<_Elme> ljust(size_t width, _Elme fillchar = ' ') const;
  basic_string<_Elme> lower(void) const;
  basic_string<_Elme> lstrip(void) const;
  basic_string<_Elme> lstrip(basic_string<_Elme> chars) const;
  void partition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3) const;
  template <typename _Iterable>
  void partition(basic_string<_Elme> sep, _Iterable &iterable) const;
  basic_string<_Elme> pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new) const;
  basic_string<_Elme> pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new, size_t count) const;
  int pyrfind(basic_string<_Elme> sub) const;
  int pyrfind(basic_string<_Elme> sub, int start) const;
  int pyrfind(basic_string<_Elme> sub, int start, int end) const;
  int rindex(basic_string<_Elme> sub) const;
  int rindex(basic_string<_Elme> sub, int start) const;
  int rindex(basic_string<_Elme> sub, int start, int end) const;
  basic_string<_Elme> rjust(size_t width, _Elme fillchar = ' ') const;
  void rpartition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3) const;
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
  bool startswith(basic_string<_Elme> suffix) const;
  bool startswith(basic_string<_Elme> suffix, int start) const;
  bool startswith(basic_string<_Elme> suffix, int start, int end) const;
  basic_string<_Elme> strip(void) const;
  basic_string<_Elme> strip(basic_string<_Elme> chars) const;
  basic_string<_Elme> swapcase(void) const;
  basic_string<_Elme> title(void) const;
  basic_string<_Elme> translate(transtable_t &table) const;
  basic_string<_Elme> upper(void) const;
  basic_string<_Elme> zfill(size_t width) const;
  basic_string<_Elme> slice(null_int_t index) const;
  basic_string<_Elme> slice(null_int_t start, null_int_t end) const;
  basic_string<_Elme> slice(null_int_t start, null_int_t end, null_int_t step) const;
  static transtable_t maketrans(std::unordered_map<basic_string<_Elme>, basic_string<_Elme>> table_map)
  {
    return basic_string<_Elme>::_maketrans(table_map);
  }
  static transtable_t maketrans(std::map<basic_string<_Elme>, basic_string<_Elme>> table_map)
  {
    return basic_string<_Elme>::_maketrans(table_map);
  }
  static transtable_t maketrans(basic_string<_Elme> from, basic_string<_Elme> to)
  {
    return basic_string<_Elme>::maketrans(from, to, "");
  }
  static transtable_t maketrans(basic_string<_Elme> from, basic_string<_Elme> to, basic_string<_Elme> delchars)
  {
    transtable_t table;
    for (int i = std::min(from.size(), to.size()); i--;)
    {
      table[from[i]] = to[i];
    }
    for (auto s : delchars)
    {
      table[s] = "";
    }
    return table;
  }
};

template <class _Elme>
basic_string<_Elme> operator+(basic_string<_Elme> r, basic_string<_Elme> l)
{
  r += l;
  return r;
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::operator*(size_t i)
{
  basic_string<_Elme> str(*this);
  if (i == 0)
  {
    str.clear();
    return str;
  }
  str.reserve(str.size() * i);
  for (; --i;)
  {
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
basic_string<_Elme> basic_string<_Elme>::operator[](std::initializer_list<null_allow::null_allow<int>> slice)
{
  basic_string<_Elme> str;
  auto in = slice.begin();
  null_int_t index1 = *in++;
  null_int_t index2 = *in++;
  null_int_t index3 = *in;

  switch (slice.size())
  {
  case 0:
    return *this;
  case 1:
    return this->slice(index1);
  case 2:
    return this->slice(index1, index2);
  default: //case 3:
    return this->slice(index1, index2, index3);
  }
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::capitalize(void) const
{
  basic_string<_Elme> str = this->lower();
  str[0] = std::toupper(str[0]);
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::center(size_t width, _Elme fillchar) const
{
  size_t r, l, len = this->size();
  if (width <= len)
    return *this;
  r = width - len;
  l = r / 2 + r % 2;

  return basic_string<_Elme>(l, fillchar) + *this + basic_string<_Elme>(r - l, fillchar);
}
template <class _Elme>
inline size_t basic_string<_Elme>::count(basic_string<_Elme> sub) const
{
  return this->count(sub, 0);
}
template <class _Elme>
inline size_t basic_string<_Elme>::count(basic_string<_Elme> sub, int start) const
{
  return this->count(sub, start, this->size());
}
template <class _Elme>
size_t basic_string<_Elme>::count(basic_string<_Elme> sub, int start, int end) const
{
  util::adjust_index(start, end, this->size());
  size_t nummatches = 0;
  int cursor = start;
  while (true)
  {
    cursor = this->pyfind(sub, cursor, end);
    if (cursor < 0)
      break;
    cursor += sub.size();
    ++nummatches;
  }

  return nummatches;
}
template <class _Elme>
inline bool basic_string<_Elme>::endswith(basic_string<_Elme> suffix) const
{
  return this->endswith(suffix, 0);
}
template <class _Elme>
inline bool basic_string<_Elme>::endswith(basic_string<_Elme> suffix, int start) const
{
  return this->endswith(suffix, start, this->size());
}
template <class _Elme>
bool basic_string<_Elme>::endswith(basic_string<_Elme> suffix, int start, int end) const
{
  int len = this->size();
  int sublen = suffix.size();

  util::adjust_index(start, end, len);
  if (end - start < sublen || start > len)
  {
    return false;
  }
  if (end - sublen > start)
  {
    start = end - sublen;
  }
  if (end - start >= sublen)
  {
    return (!std::memcmp(this->c_str() + start * sizeof(_Elme), suffix.c_str(), sublen * sizeof(_Elme)));
  }
  return false;
}
template <class _Elme>
inline basic_string<_Elme> basic_string<_Elme>::expandtabs(size_t tabsize) const
{
  return this->pyreplace("\t", basic_string<_Elme>(tabsize, ' '));
}
template <class _Elme>
inline int basic_string<_Elme>::pyfind(basic_string<_Elme> sub) const
{
  return this->pyfind(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::pyfind(basic_string<_Elme> sub, int start) const
{
  return this->pyfind(sub, start, this->size());
}
template <class _Elme>
int basic_string<_Elme>::pyfind(basic_string<_Elme> sub, int start, int end) const
{
  util::adjust_index(start, end, this->size());
  int result = this->find(sub, start);
  if (result == npos || (result + sub.size() > end))
  {
    return -1;
  }
  return result;
}
template <class _Elme>
template <class... Args>
basic_string<_Elme> basic_string<_Elme>::format(Args... args)
{
  basic_string<_Elme> str(*this);
  return this->_format(str, std::move(args)...);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::format_map(std::map<basic_string<_Elme>, basic_string<_Elme>> map)
{
  return this->_format_map(map);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::format_map(std::unordered_map<basic_string<_Elme>, basic_string<_Elme>> map)
{
  return this->_format_map(map);
}
template <class _Elme>
template <class Map>
inline basic_string<_Elme> basic_string<_Elme>::_format_map(Map &map)
{
  basic_string<_Elme> str(*this);
  basic_string<_Elme> key, val;
  int s_cursor = str.pyfind("{");
  int e_cursor = str.pyfind("}", s_cursor);
  int key_len;
  while (s_cursor != -1 && e_cursor != -1)
  {
    key_len = e_cursor - s_cursor - 1;
    key = str.substr(s_cursor + 1, key_len);

    if (map.count(key))
    {
      val = map[key];
      str.replace(s_cursor, key_len + 2, val);
      e_cursor -= key_len + 2;
      e_cursor += val.size();
    }
    s_cursor = str.pyfind("{", e_cursor);
    e_cursor = str.pyfind("}", s_cursor);
  }
  return str;
}
template <class _Elme>
inline int basic_string<_Elme>::index(basic_string<_Elme> sub) const
{
  return this->index(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::index(basic_string<_Elme> sub, int start) const
{
  return this->index(sub, start, this->size());
}
template <class _Elme>
inline int basic_string<_Elme>::index(basic_string<_Elme> sub, int start, int end) const
{
  return this->pyfind(sub, start, end);
}
template <class _Elme>
bool basic_string<_Elme>::isalnum(void) const
{
  for (auto s : *this)
  {
    if (!std::isalnum(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isalpha(void) const
{
  for (auto s : *this)
  {
    if (!std::isalpha(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isdecimal(void) const
{
  for (auto s : *this)
  {
    if (!std::isdigit(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isdigit(void) const
{
  return this->isdecimal();
}
template <class _Elme>
bool basic_string<_Elme>::islower(void) const
{
  for (auto s : *this)
  {
    if (!std::islower(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isnumeric(void) const
{
  return this->isdecimal();
}
template <class _Elme>
bool basic_string<_Elme>::isprintable(void) const
{
  for (auto s : *this)
  {
    if (!util::isprintable<_Elme>(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isspace(void) const
{
  for (auto s : *this)
  {
    if (!util::isspace<_Elme>(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::istitle(void) const
{
  if (this->size() == 0)
    return false;
  if (this->size() == 1)
    return std::isupper(this->at(0));
  bool cased = false, previous_is_cased = false;
  for (auto s : *this)
  {
    if (std::isupper(s))
    {
      if (previous_is_cased)
      {
        return false;
      }
      previous_is_cased = true;
      cased = true;
    }
    else if (std::islower(s))
    {
      if (!previous_is_cased)
      {
        return false;
      }
      previous_is_cased = true;
      cased = true;
    }
    else
    {
      previous_is_cased = false;
    }
  }
  return cased;
}
template <class _Elme>
bool basic_string<_Elme>::isupper(void) const
{
  {
    for (auto s : *this)
    {
      if (!std::isupper(s))
        return false;
    }
    return this->size() > 0;
  }
}

template <class _Elme>
template <typename _Iterable>
basic_string<_Elme> basic_string<_Elme>::join(_Iterable iterable) const
{
  basic_string<_Elme> str;
  size_t count, size = iterable.size();
  count = 0;
  for (auto &i : iterable)
  {
    str += i;
    ++count;
    if (count != size)
    {
      str += *this;
    }
  }
  return str;
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::ljust(size_t width, _Elme fillchar) const
{
  if (width <= this->size())
  {
    return *this;
  }
  return basic_string<_Elme>(width - this->size(), fillchar) + *this;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::lower(void) const
{
  {
    basic_string<_Elme> str(*this);
    for (auto &s : str)
    {
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
  basic_string<_Elme> str(*this);
  while (util::isspace(str.front()))
    str.pop_front();
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::lstrip(basic_string<_Elme> chars) const
{
  if (this->empty())
    return *this;
  if (chars.empty())
    return this->lstrip();
  basic_string<_Elme> str(*this);
  while (chars.find(str.front()) != npos)
    str.pop_front();
  return str;
}
template <class _Elme>
void basic_string<_Elme>::partition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3) const
{
  int index = this->pyfind(sep);
  if (index == -1)
  {
    dst1 = *this;
    dst2 = "";
    dst3 = "";
  }
  else
  {
    dst1 = this->substr(0, index);
    dst2 = sep;
    dst3 = this->substr(index + sep.size());
  }
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::partition(basic_string<_Elme> sep, _Iterable &iterable) const
{
  basic_string<_Elme> dst1, dst2, dst3;
  this->partition(sep, dst1, dst2, dst3);
  iterable = {dst1, dst2, dst3};
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new) const
{
  int cursor = 0;
  basic_string<_Elme> s(*this);
  size_t oldlen = old.size(), newlen = _new.size();
  cursor = s.pyfind(old, cursor);
  while (cursor != -1 && cursor <= s.size())
  {
    s.replace(cursor, oldlen, _new);
    cursor += newlen;
    if (oldlen != 0)
    {
      cursor = s.pyfind(old, cursor);
    }
    else
    {
      ++cursor;
    }
  }
  return s;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new, size_t count) const
{
  int cursor = 0;
  basic_string<_Elme> s(*this);
  size_t oldlen = old.size(), newlen = _new.size();
  cursor = s.pyfind(old, cursor);
  while (cursor != -1 && cursor <= s.size() && count > 0)
  {
    s.replace(cursor, oldlen, _new);
    cursor += newlen;
    if (oldlen != 0)
    {
      cursor = s.pyfind(old, cursor);
    }
    else
    {
      ++cursor;
    }
    --count;
  }
  return s;
}
template <class _Elme>
inline int basic_string<_Elme>::pyrfind(basic_string<_Elme> sub) const
{
  return this->pyrfind(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::pyrfind(basic_string<_Elme> sub, int start) const
{
  return this->pyrfind(sub, start, this->size());
}
template <class _Elme>
inline int basic_string<_Elme>::pyrfind(basic_string<_Elme> sub, int start, int end) const
{
  util::adjust_index(start, end, this->size());
  int result = this->rfind(sub, end);
  if (result == std::string::npos || result < start || (result + sub.size() > end))
  {
    return -1;
  }
  return result;
}
template <class _Elme>
inline int basic_string<_Elme>::rindex(basic_string<_Elme> sub) const
{
  return this->rindex(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::rindex(basic_string<_Elme> sub, int start) const
{
  return this->rindex(sub, start, this->size());
}
template <class _Elme>
inline int basic_string<_Elme>::rindex(basic_string<_Elme> sub, int start, int end) const
{
  return this->pyrfind(sub, start, end);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::rjust(size_t width, _Elme fillchar) const
{
  if (width <= this->size())
  {
    return *this;
  }
  return *this + basic_string<_Elme>(width - this->size(), fillchar);
}

template <class _Elme>
void basic_string<_Elme>::rpartition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3) const
{
  int index = this->pyrfind(sep);
  if (index == -1)
  {
    dst1 = *this;
    dst2 = "";
    dst3 = "";
  }
  else
  {
    dst1 = this->substr(0, index);
    dst2 = sep;
    dst3 = this->substr(index + sep.size());
  }
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::rpartition(basic_string<_Elme> sep, _Iterable &iterable) const
{
  basic_string<_Elme> dst1, dst2, dst3;
  this->rpartition(sep, dst1, dst2, dst3);
  iterable = {dst1, dst2, dst3};
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
  if (maxsplit < 0)
  {
    maxsplit = INT_MAX;
  }
  for (; start != end && maxsplit; ++start)
  {
    if (util::isspace(*start))
    {
      if (len != 0)
      {
        result.insert(result.begin(), this->substr(index, len));
        --maxsplit;
        index -= len;
      }
      --index;
      len = 0;
    }
    else
    {
      ++len;
    }
  }
  tmp = this->substr(0, ++index);
  tmp = tmp.rstrip();
  if (tmp.size() != 0)
  {
    result.insert(result.begin(), tmp);
  }
}

template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::rsplit(_Iterable &result, basic_string<_Elme> sep, int maxsplit) const
{
  if (sep.empty())
    return this->rsplit(result, maxsplit);
  if (result.size() != 0)
    result.clear();
  size_t index, pre_index = npos, sep_len = sep.size();
  if (maxsplit < 0)
  {
    maxsplit = INT_MAX;
  }
  for (; maxsplit--;)
  {
    index = this->rfind(sep, pre_index);
    if (index == npos)
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
    return this->lstrip();
  basic_string<_Elme> str(*this);
  while (chars.find(str.back()) != npos)
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
  if (maxsplit < 0)
  {
    maxsplit = INT_MAX;
  }
  for (; start != end && maxsplit; ++start)
  {
    if (util::isspace(*start))
    {
      if (len != 0)
      {
        result.push_back(this->substr(index, len));
        --maxsplit;
        index += len;
      }
      ++index;
      len = 0;
    }
    else
    {
      ++len;
    }
  }
  tmp = this->substr(index);
  tmp = tmp.lstrip();
  if (tmp.size() != 0)
  {
    result.push_back(tmp);
  }
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::split(_Iterable &result, basic_string<_Elme> sep, int maxsplit) const
{
  if (sep.empty())
    return this->split(result, maxsplit);
  if (result.size() != 0)
    result.clear();
  size_t index, pre_index = 0, sep_len = sep.size();
  if (maxsplit < 0)
  {
    maxsplit = INT_MAX;
  }
  for (; maxsplit--;)
  {
    index = this->find(sep, pre_index);
    if (index == npos)
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
  if (dst.size() != 0)
  {
    dst.clear();
  }
  size_t len = this->size(), i, j, eol;
  for (i = j = 0; i < len;)
  {
    while (i < len && !util::isRowBoundary<_Elme>(this->at(i)))
      ++i;
    eol = i;
    if (i < len)
    {
      if (this->at(i) == '\r' && i + 1 < len && this->at(i + 1) == '\n')
      {
        i += 2;
      }
      else
      {
        ++i;
      }
      if (keepends)
        eol = i;
    }
    dst.push_back(this->substr(j, eol - j));
    j = i;
  }
  if (j < len)
  {
    dst.push_back(this->substr(j, len - j));
  }
}
template <class _Elme>
inline bool basic_string<_Elme>::startswith(basic_string<_Elme> suffix) const
{
  return this->startswith(suffix, 0);
}
template <class _Elme>
inline bool basic_string<_Elme>::startswith(basic_string<_Elme> suffix, int start) const
{
  return this->startswith(suffix, start, this->size());
}
template <class _Elme>
bool basic_string<_Elme>::startswith(basic_string<_Elme> suffix, int start, int end) const
{
  int len = this->size();
  int sublen = suffix.size();
  const _Elme *self = this->c_str();
  const _Elme *sub = suffix.c_str();

  util::adjust_index(start, end, len);
  if (start + sublen > len)
  {
    return false;
  }
  if (end - start >= sublen)
  {
    return (!std::memcmp(self + start * sizeof(_Elme), sub, sublen * sizeof(_Elme)));
  }
  return false;
}
template <class _Elme>
inline basic_string<_Elme> basic_string<_Elme>::strip(void) const
{
  return this->lstrip();
}
template <class _Elme>
inline basic_string<_Elme> basic_string<_Elme>::strip(basic_string<_Elme> chars) const
{
  return this->lstrip(chars);
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::swapcase(void) const
{
  basic_string<_Elme> str(*this);
  for (auto &s : str)
  {
    if (std::islower(s))
    {
      s = std::toupper(s);
    }
    else if (std::isupper(s))
    {
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
  for (auto &s : str)
  {
    if (std::islower(s))
    {
      if (!previous_is_cased)
      {
        s = std::toupper(s);
      }
      previous_is_cased = true;
    }
    else if (std::isupper(s))
    {
      if (previous_is_cased)
      {
        s = std::tolower(s);
      }
      previous_is_cased = true;
    }
    else
    {
      previous_is_cased = false;
    }
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::translate(transtable_t &table) const
{
  basic_string<_Elme> str;
  for (auto s : *this)
  {
    if (table.count(s))
    {
      str += table[s];
    }
    else
    {
      str.push_back(s);
    }
  }
  return str;
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::upper(void) const
{
  basic_string<_Elme> str(*this);
  for (auto &s : str)
  {
    s = std::toupper(s);
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::zfill(size_t width) const
{
  int len = this->size();
  if (len >= width)
  {
    return *this;
  }

  basic_string<_Elme> str(*this);
  int fill = width - len;
  str = basic_string<_Elme>(fill, '0') + str;

  if (str.at(fill) == '+' || str.at(fill) == '-')
  {
    str.at(0) = str.at(fill);
    str.at(fill) = '0';
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::slice(null_int_t index) const
{
  if (index == nullptr)
  {
    return *this;
  }
  else
  {
    return this->at(index);
  }
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::slice(null_int_t start, null_int_t end) const
{
  util::adjust_index(start, end, this->size());

  if (start >= end)
  {
    return "";
  }
  return this->substr(start, end - start);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::slice(null_int_t start, null_int_t end, null_int_t step) const
{
  if (step == 0)
    return "";
  if (step == nullptr || step == 1)
    return this->slice(start, end);

  util::adjust_index(start, end, this->size());

  if (start >= end)
  {
    return "";
  }
  basic_string<_Elme> str;
  if (step > 0)
  {
    for (int i = start; i < end; i += step)
    {
      str.push_back(this->at(i));
    }
  }
  else //the case of the 3ed number is negative
  {
    for (int i = end - 1; i > start - 1; i += step)
    {
      str.push_back(this->at(i));
    }
  }
  return str;
}

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

} // namespace py
using py_string = py::string;
#endif //include garde PY_STRING_HPP