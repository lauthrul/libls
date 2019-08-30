#pragma once
#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
#define is_empty(lpstr)     ((lpstr) == 0 || (lpstr)[0] == 0)

    class lstring;
    declare_stl_obj(lstring);
    declare_stl_pointer(lstring);
    class LSLIB_API lstring : public string
    {
    public:
        lstring();
        lstring(_lchar c);
        lstring(_lpcstr str);
        lstring(const string& rh);
        virtual ~lstring();

        operator _lpcstr() const;
        lstring operator+ (_lchar c);
        lstring operator+ (_lpcstr str);
        lstring operator+ (const lstring& str);
        __ref__ lstring& operator+= (_lchar c);
        __ref__ lstring& operator+= (_lpcstr str);
        __ref__ lstring& operator+= (const lstring& str);
        __ref__ lstring& append (_lchar c);
        __ref__ lstring& append (_lpcstr str);
        __ref__ lstring& append (const lstring& str);

        template <typename T>
        _lchar operator[] (T index)
        {
            return at(index);
        }

    public:
        const bool is_space() const;
        const bool is_lower() const;
        const bool is_upper() const;
        const bool is_digital() const;
        const bool is_int() const;
        const bool is_float() const;
        const bool is_bool(bool numeric = false) const;
        const int to_int() const;
        const double to_float() const;
        const bool to_bool() const;
        lstring lower() const;
        lstring upper() const;
        __ref__ lstring& to_lower();
        __ref__ lstring& to_upper();
        __ref__ lstring& trim();
        __ref__ lstring& trim_left();
        __ref__ lstring& trim_right();
        __ref__ lstring& trim(_lchar c);
        __ref__ lstring& trim_left(_lchar c);
        __ref__ lstring& trim_right(_lchar c);
        __ref__ lstring& replace(_lchar needle, _lchar replacement, size_t index = 0, int counts = -1);
        __ref__ lstring& replace(_lpcstr needle, _lpcstr replacement, size_t index = 0, int counts = -1);
        __ref__ lstring& format(_lpcstr pfmt, ...);
        __ref__ lstring& append_format(_lpcstr pfmt, ...);
        static const int split(__out__ lstring_array& dest, _lpcstr src, _lpcstr patten, bool allow_empty = true);
        const int split(__out__ lstring_array& dest, _lpcstr patten, bool allow_empty = true) const;
    };
}
