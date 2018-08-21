#pragma once
#ifndef PY_STRING_HPP
#define PY_STRING_HPP

#include <string>
#include <cstring>
#include <cctype>
#include <cwctype>
#include <algorithm>
#include <initializer_list>
#include <sstream>

#include <iostream>
using std::cout;
using std::endl;

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

#ifndef PY_STR_UTIL
#define PY_STR_UTIL

namespace util
{
template <class _Elme>
bool isRowBoundary(_Elme c)
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
bool isprintable(_Elme c)
{
  switch (c)
  {
  case 32:
  case 33:
  case 34:
  case 35:
  case 36:
  case 37:
  case 38:
  case 39:
  case 40:
  case 41:
  case 42:
  case 43:
  case 44:
  case 45:
  case 46:
  case 47:
  case 48:
  case 49:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
  case 55:
  case 56:
  case 57:
  case 58:
  case 59:
  case 60:
  case 61:
  case 62:
  case 63:
  case 64:
  case 65:
  case 66:
  case 67:
  case 68:
  case 69:
  case 70:
  case 71:
  case 72:
  case 73:
  case 74:
  case 75:
  case 76:
  case 77:
  case 78:
  case 79:
  case 80:
  case 81:
  case 82:
  case 83:
  case 84:
  case 85:
  case 86:
  case 87:
  case 88:
  case 89:
  case 90:
  case 91:
  case 92:
  case 93:
  case 94:
  case 95:
  case 96:
  case 97:
  case 98:
  case 99:
  case 100:
  case 101:
  case 102:
  case 103:
  case 104:
  case 105:
  case 106:
  case 107:
  case 108:
  case 109:
  case 110:
  case 111:
  case 112:
  case 113:
  case 114:
  case 115:
  case 116:
  case 117:
  case 118:
  case 119:
  case 120:
  case 121:
  case 122:
  case 123:
  case 124:
  case 125:
  case 126:
    return true;
  default:
    return false;
  }
}

template <class _Elme>
bool isspace(_Elme c)
{
  switch (c)
  {
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 28:
  case 29:
  case 30:
  case 31:
  case 32:
    return true;
  default:
    return false;
  }
}

} // namespace util
#endif

#ifndef PY_SLICE
#define PY_SLICE

using nint_t = null_allow::null_allow<int>;

#endif

template <class _Elme>
class basic_string : public std::basic_string<_Elme>
{
private:
  size_t _back_index(int index) { return this->size() + index; }
  size_t _index_normalize(int index)
  {
    index = index < 0 ? index : _back_index(index);
    return index > this->size() ? this->size() : index;
  }
  int size_i(void) { return this->size(); }
  void pop_front(void)
  {
    if (!empty())
      this->erase(0, 1);
  }
  basic_string<_Elme> _format(basic_string<_Elme> &_Str){return _Str; }
  template <class Head, class... Tail>
  basic_string<_Elme> _format(basic_string<_Elme> &_Str, Head head, Tail... tail)
  {
    std::basic_stringstream<_Elme> stm;
    stm << head;
    int index = _Str.pyfind("{}");
    if (index == -1)
    {
      return _Str;
    }
    _Str = _Str.pyreplace("{}", stm.str(), 1);
    return this->_format(_Str,std::move(tail)...);
  }

public:
  using std::basic_string<_Elme>::basic_string;
  using std::basic_string<_Elme>::operator+=;
  using std::basic_string<_Elme>::operator=;
  basic_string<_Elme>() : std::basic_string<_Elme>(){};
  basic_string<_Elme>(std::basic_string<_Elme> _Str) : std::basic_string<_Elme>(_Str){};
  //override//
  const _Elme &at(int _Index) const { return (_Index < 0) ? std::basic_string<_Elme>::at(_back_index(_Index)) : std::basic_string<_Elme>::at(_Index); }
  _Elme &at(int _Index) { return (_Index < 0) ? std::basic_string<_Elme>::at(_back_index(_Index)) : std::basic_string<_Elme>::at(_Index); }
  const _Elme &operator[](int _Index) const { return this->at(_Index); }
  _Elme &operator[](int _Index) { return this->at(_Index); }
  /////

  basic_string<_Elme> operator*(size_t i);
  basic_string<_Elme> &operator*=(size_t i);
  basic_string<_Elme> operator[](std::initializer_list<null_allow::null_allow<int>> slice);
  basic_string<_Elme> capitalize(void);
  basic_string<_Elme> center(size_t width, _Elme fillchar = ' ');
  size_t count(basic_string<_Elme> sub);
  size_t count(basic_string<_Elme> sub, int start);
  size_t count(basic_string<_Elme> sub, int start, int end);
  bool endswith(basic_string<_Elme> suffix);
  bool endswith(basic_string<_Elme> suffix, int start);
  bool endswith(basic_string<_Elme> suffix, int start, int end);
  basic_string<_Elme> expandtabs(size_t tabsize = 8);
  int pyfind(basic_string<_Elme> sub);
  int pyfind(basic_string<_Elme> sub, int start);
  int pyfind(basic_string<_Elme> sub, int start, int end);
  template <class Head, class... Tail>
  basic_string<_Elme> format(Head head, Tail ... tail);
  int index(basic_string<_Elme> sub);
  int index(basic_string<_Elme> sub, int start);
  int index(basic_string<_Elme> sub, int start, int end);
  bool isalnum(void);
  bool isalpha(void);
  bool isdecimal(void);
  bool isdigit(void);
  bool islower(void);
  bool isnumeric(void);
  bool isprintable(void);
  bool isspace(void);
  bool istitle(void);
  bool isupper(void);
  template <typename _Iterable>
  basic_string<_Elme> join(_Iterable iterable);
  basic_string<_Elme> ljust(size_t width, _Elme fillchar = ' ');
  basic_string<_Elme> lower(void);
  basic_string<_Elme> lstrip(void);
  basic_string<_Elme> lstrip(basic_string<_Elme> chars);
  void partition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3);
  template <typename _Iterable>
  void partition(basic_string<_Elme> sep, _Iterable &iterable);
  basic_string<_Elme> pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new);
  basic_string<_Elme> pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new, size_t count);
  int pyrfind(basic_string<_Elme> sub);
  int pyrfind(basic_string<_Elme> sub, int start);
  int pyrfind(basic_string<_Elme> sub, int start, int end);
  int rindex(basic_string<_Elme> sub);
  int rindex(basic_string<_Elme> sub, int start);
  int rindex(basic_string<_Elme> sub, int start, int end);
  basic_string<_Elme> rjust(size_t width, _Elme fillchar = ' ');
  void rpartition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3);
  template <typename _Iterable>
  void rpartition(basic_string<_Elme> sep, _Iterable &iterable);

  template <typename _Iterable>
  void rsplit(_Iterable &dst, int maxsplit = -1);
  template <typename _Iterable>
  void rsplit(_Iterable &dst, basic_string<_Elme> sep, int maxsplit = -1);

  basic_string<_Elme> rstrip(void);
  basic_string<_Elme> rstrip(basic_string<_Elme> chars);

  template <typename _Iterable>
  void split(_Iterable &dst, int maxsplit = -1);
  template <typename _Iterable>
  void split(_Iterable &dst, basic_string<_Elme> sep, int maxsplit = -1);

  template <typename _Iterable>
  void splitlines(_Iterable &dst, bool keepends = false);
  bool startswith(basic_string<_Elme> suffix);
  bool startswith(basic_string<_Elme> suffix, int start);
  bool startswith(basic_string<_Elme> suffix, int start, int end);
  basic_string<_Elme> strip(void);
  basic_string<_Elme> strip(basic_string<_Elme> chars);
  basic_string<_Elme> swapcase(void);
  basic_string<_Elme> title(void);
  basic_string<_Elme> upper(void);
  basic_string<_Elme> zfill(size_t width);
  basic_string<_Elme> slice(null_allow::null_allow<int> index);
  basic_string<_Elme> slice(null_allow::null_allow<int> start, null_allow::null_allow<int> end);
  basic_string<_Elme> slice(null_allow::null_allow<int> start, null_allow::null_allow<int> end, null_allow::null_allow<int> step);
};

template <class _Elme>
basic_string<_Elme> operator+(basic_string<_Elme> r, basic_string<_Elme> l)
{
  r += l;
  return r;
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
  null_allow::null_allow<int> index1 = *in;
  ++in;
  null_allow::null_allow<int> index2 = *in;
  ++in;
  null_allow::null_allow<int> index3 = *in;
  if (slice.size() == 0)
  {
    return *this;
  }
  else if (slice.size() == 1)
  {
    if (index1 == nullptr)
    {
      return *this;
    }
    else
    {
      str.push_back(this->at(index1));
      return str;
    }
  }
  else if (slice.size() == 2)
  {
    index1 = index1 == nullptr ? 0 : index1;
    index1 = index1 < 0 ? _back_index(index1) : index1;

    index2 = (index2 == nullptr) || (index2 > this->size_i()) ? this->size_i() : index2;
    index2 = (index2 < 0) ? _back_index(index2) : index2;

    if (index1 >= index2)
    {
      return str;
    }

    for (int i = index1; i < index2; i++)
    {
      str.push_back(this->at(i));
    }
    return str;
  }
  else
  {
    if (index3 == 0)
    {
      return str;
    }
    index1 = index1 == nullptr ? 0 : index1;
    index1 = index1 < 0 ? _back_index(index1) : index1;

    index2 = (index2 == nullptr) || (index2 > this->size_i()) ? this->size_i() : index2;
    index2 = (index2 < 0) ? _back_index(index2) : index2;

    index3 = index3 == nullptr ? 1 : index3;

    if (index3 > 0)
    {
      for (int i = index1; i < index2; i += index3)
      {
        str.push_back(this->at(i));
      }
    }
    else //the case of the 3ed number is negative
    {
      for (int i = index1 - 1; std::abs(i) < index2 + 1; i += index3)
      {
        str.push_back(this->at(i));
      }
    }
    return str;
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::capitalize(void)
{
  basic_string<_Elme> str = this->lower();
  str[0] = std::toupper(str[0]);
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::center(size_t width, _Elme fillchar)
{
  size_t r, l, len = this->size();
  if (width <= len)
    return *this;
  r = width - len;
  l = r / 2 + r % 2;

  return basic_string<_Elme>(l, fillchar) + *this + basic_string<_Elme>(r - l, fillchar);
}
template <class _Elme>
inline size_t basic_string<_Elme>::count(basic_string<_Elme> sub)
{
  return this->count(sub, 0);
}
template <class _Elme>
inline size_t basic_string<_Elme>::count(basic_string<_Elme> sub, int start)
{
  return this->count(sub, start, this->size());
}
template <class _Elme>
size_t basic_string<_Elme>::count(basic_string<_Elme> sub, int start, int end)
{
  adjust_index(start, end, this->size());
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
inline bool basic_string<_Elme>::endswith(basic_string<_Elme> suffix)
{
  return this->endswith(suffix, 0);
}
template <class _Elme>
inline bool basic_string<_Elme>::endswith(basic_string<_Elme> suffix, int start)
{
  return this->endswith(suffix, start, this->size());
}
template <class _Elme>
bool basic_string<_Elme>::endswith(basic_string<_Elme> suffix, int start, int end)
{
  int len = this->size();
  int sublen = suffix.size();

  adjust_index(start, end, len);
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
inline basic_string<_Elme> basic_string<_Elme>::expandtabs(size_t tabsize)
{
  return this->pyreplace("\t", basic_string<_Elme>(tabsize, ' '));
}
template <class _Elme>
inline int basic_string<_Elme>::pyfind(basic_string<_Elme> sub)
{
  return this->pyfind(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::pyfind(basic_string<_Elme> sub, int start)
{
  return this->pyfind(sub, start, this->size());
}
template <class _Elme>
int basic_string<_Elme>::pyfind(basic_string<_Elme> sub, int start, int end)
{
  adjust_index(start, end, this->size());
  int result = this->find(sub, start);
  if (result == npos || (result + sub.size() > end))
  {
    return -1;
  }
  return result;
}
template <class _Elme>
template <class Head, class... Tail>
basic_string<_Elme> basic_string<_Elme>::format(Head head, Tail... tail)
{
  std::basic_stringstream<_Elme> stm;
  stm << head;

  basic_string<_Elme> str(*this);
  int index = str.pyfind("{}");
  if (index == -1)
  {
    return str;
  }
  str = str.pyreplace("{}", stm.str(), 1);
  return this->_format(str, std::move(tail)...);
}
template <class _Elme>
inline int basic_string<_Elme>::index(basic_string<_Elme> sub)
{
  return this->index(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::index(basic_string<_Elme> sub, int start)
{
  return this->index(sub, start, this->size());
}
template <class _Elme>
inline int basic_string<_Elme>::index(basic_string<_Elme> sub, int start, int end)
{
  return this->pyfind(sub, start, end);
}
template <class _Elme>
bool basic_string<_Elme>::isalnum(void)
{
  for (auto s : *this)
  {
    if (!std::isalnum(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isalpha(void)
{
  for (auto s : *this)
  {
    if (!std::isalpha(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isdecimal(void)
{
  for (auto s : *this)
  {
    if (!std::isdigit(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isdigit(void)
{
  return this->isdecimal();
}
template <class _Elme>
bool basic_string<_Elme>::islower(void)
{
  for (auto s : *this)
  {
    if (!std::islower(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isnumeric(void)
{
  return this->isdecimal();
}
template <class _Elme>
bool basic_string<_Elme>::isprintable(void)
{
  for (auto s : *this)
  {
    if (!util::isprintable<_Elme>(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::isspace(void)
{
  for (auto s : *this)
  {
    if (!util::isspace<_Elme>(s))
      return false;
  }
  return this->size() > 0;
}
template <class _Elme>
bool basic_string<_Elme>::istitle(void)
{
  if (this->size() == 0)
    return false;
  if (this->size() == 1)
    return ::isupper(str[0]);
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
bool basic_string<_Elme>::isupper(void)
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
basic_string<_Elme> basic_string<_Elme>::join(_Iterable iterable)
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
basic_string<_Elme> basic_string<_Elme>::ljust(size_t width, _Elme fillchar)
{
  if (width <= this->size())
  {
    return *this;
  }
  return basic_string<_Elme>(width - this->size(), fillchar) + *this;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::lower(void)
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
basic_string<_Elme> basic_string<_Elme>::lstrip(void)
{
  if (this->empty())
    return *this;
  basic_string<_Elme> str(*this);
  while (true)
  {
    if (util::isspace(str.front()))
      str.pop_front();
    else
      break;
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::lstrip(basic_string<_Elme> chars)
{
  if (this->empty())
    return *this;
  if (chars.empty())
    return this->lstrip();
  basic_string<_Elme> str(*this);
  while (true)
  {
    if (chars.pyfind(basic_string<_Elme>(1, str.front())) != -1)
      str.pop_front();
    else
      break;
  }
  return str;
}
template <class _Elme>
void basic_string<_Elme>::partition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3)
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
void basic_string<_Elme>::partition(basic_string<_Elme> sep, _Iterable &iterable)
{
  basic_string<_Elme> dst1, dst2, dst3;
  this->partition(sep, dst1, dst2, dst3);
  iterable = {dst1, dst2, dst3};
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new)
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
basic_string<_Elme> basic_string<_Elme>::pyreplace(basic_string<_Elme> old, basic_string<_Elme> _new, size_t count)
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
inline int basic_string<_Elme>::pyrfind(basic_string<_Elme> sub)
{
  return this->pyrfind(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::pyrfind(basic_string<_Elme> sub, int start)
{
  return this->pyrfind(sub, start, this->size());
}
template <class _Elme>
inline int basic_string<_Elme>::pyrfind(basic_string<_Elme> sub, int start, int end)
{
  adjust_index(start, end, this->size());
  int result = this->rfind(sub, end);
  if (result == std::string::npos || result < start || (result + sub.size() > end))
  {
    return -1;
  }
  return result;
}
template <class _Elme>
inline int basic_string<_Elme>::rindex(basic_string<_Elme> sub)
{
  return this->rindex(sub, 0);
}
template <class _Elme>
inline int basic_string<_Elme>::rindex(basic_string<_Elme> sub, int start)
{
  return this->rindex(sub, start, this->size());
}
template <class _Elme>
inline int basic_string<_Elme>::rindex(basic_string<_Elme> sub, int start, int end)
{
  return this->pyrfind(sub, start, end);
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::rjust(size_t width, _Elme fillchar)
{
  if (width <= this->size())
  {
    return *this;
  }
  return *this + basic_string<_Elme>(width - this->size(), fillchar);
}

template <class _Elme>
void basic_string<_Elme>::rpartition(basic_string<_Elme> sep, basic_string<_Elme> &dst1, basic_string<_Elme> &dst2, basic_string<_Elme> &dst3)
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
void basic_string<_Elme>::rpartition(basic_string<_Elme> sep, _Iterable &iterable)
{
  basic_string<_Elme> dst1, dst2, dst3;
  this->rpartition(sep, dst1, dst2, dst3);
  iterable = {dst1, dst2, dst3};
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::rsplit(_Iterable &result, int maxsplit)
{
  if (result.size() != 0)
    result.clear();
  auto start = this->rbegin();
  auto end = this->rend();
  int index = this->size() - 1, len = 0;
  basic_string<_Elme> tmp;
  if (maxsplit < 0)
  {
    for (; start != end; ++start)
    {
      if (util::isspace(*start))
      {
        if (len != 0)
        {
          result.push_back(this->substr(index, len));
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
  }
  else
  {
    for (; start != end && maxsplit; ++start)
    {
      if (util::isspace(*start))
      {
        if (len != 0)
        {
          result.push_back(this->substr(index, len));
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
  }
  tmp = this->substr(0, index);
  tmp = tmp.rstrip();
  if (tmp.size() != 0)
  {
    result.push_back(tmp);
  }
}

template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::rsplit(_Iterable &result, basic_string<_Elme> sep, int maxsplit)
{
  if (sep.empty())
    return this->rsplit(result, maxsplit);
  if (result.size() != 0)
    result.clear();
  basic_string<_Elme> tmp1, tmp2, tmp3;
  basic_string<_Elme> str(*this);
  if (maxsplit < 0)
  {
    while (1)
    {
      str.rpartition(sep, tmp1, tmp2, tmp3);
      if (str == tmp1)
        break;
      str = tmp1;
      result.insert(result.begin(), tmp3);
    }
    result.insert(result.begin(), tmp1);
  }
  else
  {
    for (; maxsplit--;)
    {
      str.rpartition(sep, tmp1, tmp2, tmp3);
      if (str == tmp1)
        break;
      str = tmp1;
      result.insert(result.begin(), tmp3);
    }
    result.insert(result.begin(), tmp1);
  }
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::rstrip(void)
{
  if (this->empty())
    return *this;
  basic_string<_Elme> str(*this);
  while (1)
  {
    if (util::isspace(str.back()))
      str.pop_back();
    else
      break;
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::rstrip(basic_string<_Elme> chars)
{
  if (this->empty())
    return *this;
  if (chars.empty())
    return this->lstrip();
  basic_string<_Elme> str(*this);
  while (1)
  {
    if (chars.pyfind(basic_string<_Elme>(1, str.back())) != -1)
      str.pop_back();
    else
      break;
  }
  return str;
}

template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::split(_Iterable &result, int maxsplit)
{
  if (result.size() != 0)
    result.clear();
  auto start = this->begin();
  auto end = this->end();
  int index = 0, len = 0;
  basic_string<_Elme> tmp;
  if (maxsplit < 0)
  {
    for (; start != end; ++start)
    {
      if (util::isspace(*start))
      {
        if (len != 0)
        {
          result.push_back(this->substr(index, len));
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
  }
  else
  {
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
void basic_string<_Elme>::split(_Iterable &result, basic_string<_Elme> sep, int maxsplit)
{
  if (sep.empty())
    return this->split(result, maxsplit);
  if (result.size() != 0)
    result.clear();
  basic_string<_Elme> tmp1, tmp2, tmp3;
  basic_string<_Elme> str(*this);
  if (maxsplit < 0)
  {
    while (1)
    {
      str.partition(sep, tmp1, tmp2, tmp3);
      if (str == tmp1)
        break;
      str = tmp3;
      result.push_back(tmp1);
    }
    result.push_back(tmp3);
  }
  else
  {
    for (; maxsplit--;)
    {
      str.partition(sep, tmp1, tmp2, tmp3);
      if (str == tmp1)
        break;
      str = tmp3;
      result.push_back(tmp1);
    }
    result.push_back(tmp3);
  }
}
template <class _Elme>
template <typename _Iterable>
void basic_string<_Elme>::splitlines(_Iterable &dst, bool keepends)
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
bool basic_string<_Elme>::startswith(basic_string<_Elme> suffix)
{
  return this->startswith(suffix, 0);
}
template <class _Elme>
bool basic_string<_Elme>::startswith(basic_string<_Elme> suffix, int start)
{
  return this->startswith(suffix, start, this->size());
}
template <class _Elme>
bool basic_string<_Elme>::startswith(basic_string<_Elme> suffix, int start, int end)
{
  int len = this->size();
  int sublen = suffix.size();
  const _Elme *self = this->c_str();
  const _Elme *sub = suffix.c_str();

  adjust_index(start, end, len);
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
basic_string<_Elme> basic_string<_Elme>::strip(void)
{
  return this->lstrip();
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::strip(basic_string<_Elme> chars)
{
  return this->lstrip(chars);
}

template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::swapcase(void)
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
basic_string<_Elme> basic_string<_Elme>::title(void)
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
basic_string<_Elme> basic_string<_Elme>::upper(void)
{
  basic_string<_Elme> str(*this);
  for (auto &s : str)
  {
    s = std::toupper(s);
  }
  return str;
}
template <class _Elme>
basic_string<_Elme> basic_string<_Elme>::zfill(size_t width)
{
  int len = this->size();
  if (len >= width)
  {
    return *this;
  }

  basic_string<_Elme> str(*this);
  int fill = width - len;
  str = std::string(fill, '0') + str;

  if (str.at(fill) == '+' || str.at(fill) == '-')
  {
    str.at(0) = str.at(fill);
    str.at(fill) = '0';
  }
  return str;
}
/*
basic_string<_Elme> slice(null_allow::null_allow<int> index);
basic_string<_Elme> slice(null_allow::null_allow<int> start, null_allow::null_allow<int> end);
basic_string<_Elme> slice(null_allow::null_allow<int> start, null_allow::null_allow<int> end, null_allow::null_allow<int> step);
template<class _Elme>
template<class _Elme>
template<class _Elme>

*/

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

} // namespace py
#endif //include garde PY_STRING_HPP