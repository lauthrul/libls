#pragma once
#include <algorithm>
#include <functional>

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
#define is_empty(lpstr)		((lpstr) == 0 || (lpstr)[0] == 0)

	class lstring;
	stl_register_obj(lstring);
	stl_register_pointer(lstring);
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
		_ref lstring& to_lower();
		_ref lstring& to_upper();
		_ref lstring& trim();
		_ref lstring& trim_left();
		_ref lstring& trim_right();
		_ref lstring& trim(_lchar c);
		_ref lstring& trim_left(_lchar c);
		_ref lstring& trim_right(_lchar c);
		_ref lstring& replace(_lchar needle, _lchar replacement, size_t index = 0, int counts = -1);
		_ref lstring& replace(_lpcstr needle, _lpcstr replacement, size_t index = 0, int counts = -1);
		_ref lstring& format(_lpcstr pfmt, ...);
		_ref lstring& append_format(_lpcstr pfmt, ...);
		static const int split(__out lstring_array& dest, _lpcstr src, _lpcstr patten);
		const int split(__out lstring_array& dest, _lpcstr patten) const;
	};
}
