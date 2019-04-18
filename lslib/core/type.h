#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
using namespace std;

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
typedef byte*				_lpbyte;
typedef unsigned short		_lword;
typedef unsigned long		_ldword;
typedef _lword*				_lpword;
typedef _ldword*			_lpdword;
typedef unsigned char		_luchar;
typedef char				_lchar;
typedef char*				_lpstr;
typedef const char*			_lpcstr;
typedef wchar_t				_lwchar;
typedef wchar_t*			_lpwchar;
typedef const wchar_t*		_lpcwchar;

#define stl_register_obj(type) \
	typedef vector<type>	type##_array; \
	typedef list<type>		type##_list; \
	typedef set<type>		type##_set;

#define stl_register_pointer(type) \
	typedef vector<type*>	p##type##_array; \
	typedef list<type*>		p##type##_list; \
	typedef set<type*>		p##type##_set;
}