#include "stdafx.h"
#include "string.h"

//////////////////////////////////////////////////////////////////////////
#define isspace(c)          ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n' || (c) == '\v' || (c) == '\f')

//////////////////////////////////////////////////////////////////////////
namespace lslib
{

    lstring::lstring()
    {
    }

    lstring::lstring(_lchar c) : string(1, c)
    {
    }

    lstring::lstring(_lpcstr str)
    {
        if (str != NULL)
        {
            _Tidy();
            assign(str);
        }
    }

    lstring::lstring(const string& str) : string(str)
    {
    }

    lstring::~lstring()
    {
    }

    lstring::operator _lpcstr() const
    {
        return c_str();
    }

    lstring lstring::operator+(_lchar c)
    {
        return lstring(*this) += c;
    }

    lstring lstring::operator+(_lpcstr str)
    {
        return lstring(*this) += str;
    }

    lstring lstring::operator+(const lstring& str)
    {
        return lstring(*this) += str;
    }

    _lchar lstring::operator[](size_t index)
    {
        return (_Myptr()[index]);
    }

    const bool lstring::is_space() const
    {
        _lpcstr p = data();
        while (*p != 0)
        {
            if (!isspace(*p))
                return false;
            p++;
        }
        return true;
    }

    const bool lstring::is_lower() const
    {
        _lpcstr p = data();
        while (*p != 0)
        {
            if (!islower(*p))
                return false;
            p++;
        }
        return true;
    }

    const bool lstring::is_upper() const
    {
        _lpcstr p = data();
        while (*p != 0)
        {
            if (!isupper(*p))
                return false;
            p++;
        }
        return true;
    }

    const bool lstring::is_digital() const
    {
        _lpcstr p = data();
        while (*p != 0)
        {
            if (!isdigit(*p))
                return false;
            p++;
        }
        return true;
    }

    const bool lstring::is_int() const
    {
        _lpcstr p = data();
        if (*p != '-' && !isdigit(*p))
            return false;
        p++;
        while (*p != 0)
        {
            if (!isdigit(*p))
                return false;
            p++;
        }
        return true;
    }

    const bool lstring::is_float() const
    {
        _lpcstr p = data();
        int dotcnt = 0;
        while (*p != 0)
        {
            if (*p == '.')
                dotcnt++;
            if (!isdigit(*p) && dotcnt > 1)
                return false;
            p++;
        }
        return true;
    }

    const bool lstring::is_bool(bool numeric /*= false*/) const
    {
        lstring str = lower();
        if (str == "true" || str == "false")
            return true;
        if (numeric)
        {
            if (str.length() == 1 && (str == "1" || str == "0"))
                return true;
        }
        return false;
    }

    const int lstring::to_int() const
    {
        if (!is_int()) return 0;
        return atoi(data());
    }

    const double lstring::to_float() const
    {
        if (!is_float()) return 0;
        return atof(data());
    }

    const bool lstring::to_bool() const
    {
        if (!is_bool()) return false;
        lstring str = lower();
        if (str == "true" || str == "1")
            return true;
        return false;
    }

    lstring lstring::lower() const
    {
        lstring str = data();
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    lstring lstring::upper() const
    {
        lstring str = data();
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    _ref lstring& lstring::to_lower()
    {
        transform(begin(), end(), begin(), ::tolower);
        return *this;
    }

    _ref lstring& lstring::to_upper()
    {
        transform(begin(), end(), begin(), ::toupper);
        return *this;
    }

    _ref lstring& lstring::trim()
    {
        trim_left();
        trim_right();
        return *this;
    }

    _ref lstring& lstring::trim_left()
    {
        iterator it = begin();
        while (it != end() && isspace(*it))
            it = erase(it);
        return *this;
    }

    _ref lstring& lstring::trim_right()
    {
        reverse_iterator it = rbegin();
        while (it != rend() && isspace(*it))
            it = reverse_iterator(erase((++it).base()));
        return *this;
    }

    _ref lstring& lstring::trim(_lchar c)
    {
        trim_left(c);
        trim_right(c);
        return *this;
    }

    _ref lstring& lstring::trim_left(_lchar c)
    {
        iterator it = begin();
        while (it != end() && c == (*it))
            it = erase(it);
        return *this;
    }

    _ref lstring& lstring::trim_right(_lchar c)
    {
        reverse_iterator it = rbegin();
        while (it != rend() && c == (*it))
            it = reverse_iterator(erase((++it).base()));
        return *this;
    }

    _ref lstring& lstring::replace(_lchar needle, _lchar replacement, size_t index /*= 0*/, int counts /*= -1*/)
    {
        if (needle == replacement)
            return *this;

        iterator p = begin();
        size_t replace_index = 0;
        int replace_count = 0;
        while (p != end())
        {
            if (*p == needle)
            {
                if (replace_index < index)
                {
                    p++;
                    replace_index++;
                    continue;
                }

                if (counts > 0 && replace_count >= counts)
                    break;

                *p = replacement;
                replace_index++;
                replace_count++;
            }
            p++;
        }
        return *this;
    }

    _ref lstring& lstring::replace(_lpcstr needle, _lpcstr replacement, size_t index /*= 0*/, int counts /*= -1*/)
    {
        if (is_empty(needle) || is_empty(replacement))
            return *this;
        if (strcmp(needle, replacement) == 0)
            return *this;

        size_t needle_len = strlen(needle);
        size_t replacement_len = strlen(replacement);
        int replace_count = 0;

        // calculate new buffer size
        _lpcstr p = data();
        while (*p != 0)
        {
            if ( strncmp(p, needle, needle_len) == 0 )  replace_count++;
            p++;
        }

        // new buffer
        size_t new_size = length() - replace_count * needle_len + replace_count * replacement_len;

        _lpstr pbuffer = new _lchar[new_size + 1];
        memset(pbuffer, 0, new_size + 1);

        // process of replacement
        _lpstr insert_point = pbuffer;
        _lpcstr start = data();
        _lpcstr tmp = start;

        replace_count = 0;
        size_t replace_index = 0;
        while (1)
        {
            _lpcstr p = strstr(tmp, needle);
            if (p != NULL && replace_index < index)
            {
                // copy part before needle
                memcpy(insert_point, tmp, p - tmp);
                insert_point += p - tmp;

                // copy needle
                memcpy(insert_point, needle, needle_len);
                insert_point += needle_len;

                // adjust pointers, move on
                tmp = p + needle_len;
                replace_index++;
                continue;
            }

            // walked past last occurrence of needle; copy remaining part
            if (p == NULL || (counts > 0 && replace_count >= counts))
            {
                strcpy(insert_point, tmp);
                break;
            }

            // copy part before needle
            memcpy(insert_point, tmp, p - tmp);
            insert_point += p - tmp;

            // copy replacement string
            memcpy(insert_point, replacement, replacement_len);
            insert_point += replacement_len;

            // adjust pointers, move on
            tmp = p + needle_len;
            replace_index++;
            replace_count++;
        }

        (*this) = string(pbuffer);
        delete[] pbuffer;
        return *this;
    }

    _ref lstring& lstring::format(_lpcstr pfmt, ...)
    {
        va_list args;
        va_start(args, pfmt);
        int len = _vsntprintf(NULL, 0, pfmt, args);
        _lpstr pbuf = new _lchar[len + 1];
        memset(pbuf, 0, len + 1);
        _vsntprintf(pbuf, len, pfmt, args);
        va_end(args);
        *this = pbuf;
        delete[] pbuf;
        return *this;
    }

    _ref lstring& lstring::append_format(_lpcstr pfmt, ...)
    {
        va_list args;
        va_start(args, pfmt);
        int len = _vsntprintf(NULL, 0, pfmt, args);
        _lpstr pbuf = new _lchar[len + 1];
        memset(pbuf, 0, len + 1);
        _vsntprintf(pbuf, len, pfmt, args);
        va_end(args);
        *this += pbuf;
        delete[] pbuf;
        return *this;
    }

    const int lstring::split(__out lstring_array& dest, _lpcstr src, _lpcstr patten)
    {
        if (is_empty(src) || is_empty(patten))
        {
            dest.push_back(src);
            return dest.size();
        }

        _lpstr pbuff = NULL;
        _lpcstr last_pos = src;
        _lpcstr p = last_pos;
        int size = 0;
        while (p != NULL)
        {
            p = strstr(last_pos, patten);
            if (p != NULL)
            {
                size = p - last_pos;
                pbuff = new _lchar[size + 1];
                memset(pbuff, 0, size + 1);
                strncpy(pbuff, last_pos, size);
                dest.push_back(pbuff);
                delete[] pbuff;
                last_pos = p + strlen(patten);
                if (last_pos[0] == 0)  dest.push_back(last_pos);
            }
        }
        if (last_pos[0] != 0) dest.push_back(last_pos);

        return dest.size();
    }

    const int lstring::split(__out lstring_array& dest, _lpcstr patten) const
    {
        return split(dest, *this, patten);
    }

} // end of lslib