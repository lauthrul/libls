#include "stdafx.h"
#include <cstdarg>

//////////////////////////////////////////////////////////////////////////
#define isspace(c)          ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n' || (c) == '\v' || (c) == '\f')

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    //////////////////////////////////////////////////////////////////////////
    lstring::lstring()
    {

    }

    lstring::lstring(_lpcstr lpstr) : str(lpstr)
    {

    }

    lstring::lstring(_lchar c, size_t counts /*= 1*/)
    {
        str = string(counts, c);
    }

    lstring::lstring(const string& str)
    {
        this->str = str;
    }

    lstring::~lstring()
    {

    }

    lstring::operator _lpcstr()
    {
        return str.c_str();
    }

    //////////////////////////////////////////////////////////////////////////
    namespace strtool
    {
        LSLIB_API const bool is_empty(_lpcstr str)
        {
            return (str == 0 || (str)[0] == 0);
        }

        LSLIB_API const bool is_empty(const string& str)
        {
            return is_empty(str.c_str());
        }

        LSLIB_API const bool is_space(_lpcstr str)
        {
            _lpcstr p = str;
            while (*p != 0)
            {
                if (!isspace(*p))
                    return false;
                p++;
            }
            return true;
        }

        LSLIB_API const bool is_space(const string& str)
        {
            return is_space(str.c_str());
        }

        LSLIB_API const bool is_lower(_lpcstr str)
        {
            _lpcstr p = str;
            while (*p != 0)
            {
                if (!islower(*p))
                    return false;
                p++;
            }
            return true;
        }

        LSLIB_API const bool is_lower(const string& str)
        {
            return is_lower(str.c_str());
        }

        LSLIB_API const bool is_upper(_lpcstr str)
        {
            _lpcstr p = str;
            while (*p != 0)
            {
                if (!isupper(*p))
                    return false;
                p++;
            }
            return true;
        }

        LSLIB_API const bool is_upper(const string& str)
        {
            return is_upper(str.c_str());
        }

        LSLIB_API const bool is_digital(_lpcstr str)
        {
            _lpcstr p = str;
            while (*p != 0)
            {
                if (!isdigit(*p))
                    return false;
                p++;
            }
            return true;
        }

        LSLIB_API const bool is_digital(const string& str)
        {
            return is_digital(str.c_str());
        }

        LSLIB_API const bool is_int(_lpcstr str)
        {
            _lpcstr p = str;
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

        LSLIB_API const bool is_int(const string& str)
        {
            return is_int(str.c_str());
        }

        LSLIB_API const bool is_float(_lpcstr str)
        {
            _lpcstr p = str;
            if (*p != '-' && *p != '.' && !isdigit(*p))
                return false;
            p++;
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

        LSLIB_API const bool is_float(const string& str)
        {
            return is_float(str.c_str());
        }

        LSLIB_API const bool is_bool(_lpcstr str, bool numeric /*= false*/)
        {
            string str_lower = lower(str);
            if (str_lower == "true" || str_lower == "false")
                return true;
            if (numeric)
            {
                if (str_lower.length() == 1 && (str_lower == "1" || str_lower == "0"))
                    return true;
            }
            return false;
        }

        LSLIB_API const bool is_bool(const string& str, bool numeric /*= false*/)
        {
            return is_bool(str.c_str(), numeric);
        }

        LSLIB_API const int to_int(_lpcstr str)
        {
            if (!is_int(str)) return 0;
            return atoi(str);
        }

        LSLIB_API const int to_int(const string& str)
        {
            return to_int(str.c_str());
        }

        LSLIB_API const double to_float(_lpcstr str)
        {
            if (!is_float(str)) return 0;
            return atof(str);
        }

        LSLIB_API const double to_float(const string& str)
        {
            return to_float(str.c_str());
        }

        LSLIB_API const bool to_bool(_lpcstr str)
        {
            if (!is_bool(str)) return false;
            string str_lower = lower(str);
            if (str_lower == "true" || str_lower == "1")
                return true;
            return false;
        }

        LSLIB_API const bool to_bool(const string& str)
        {
            return to_bool(str.c_str());
        }

        LSLIB_API string from_int(int value)
        {
            string str = strtool::format("%d", value);
            return str;
        }

        LSLIB_API string from_float(double value, int bit)
        {
            string format = strtool::format("%%0%df", bit);
            string str = strtool::format(format.c_str(), value);
            return str;
        }

        LSLIB_API string from_bool(bool value)
        {
            return value ? "true" : "false";
        }

        LSLIB_API string lower(_lpcstr str)
        {
            string str_wrapper = str;
            transform(str_wrapper.begin(), str_wrapper.end(), str_wrapper.begin(), ::tolower);
            return str;
        }

        LSLIB_API string lower(const string& str)
        {
            return lower(str.c_str());
        }

        LSLIB_API string upper(_lpcstr str)
        {
            string str_wrapper = str;
            transform(str_wrapper.begin(), str_wrapper.end(), str_wrapper.begin(), ::toupper);
            return str;
        }

        LSLIB_API string upper(const string& str)
        {
            return upper(str.c_str());
        }

        LSLIB_API string trim(_lpcstr str)
        {
            string str_wrapper = str;
            str_wrapper = trim_left(str_wrapper.c_str());
            str_wrapper = trim_right(str_wrapper.c_str());
            return str_wrapper;
        }

        LSLIB_API string trim(const string& str)
        {
            return trim(str.c_str());
        }

        LSLIB_API string trim_left(_lpcstr str)
        {
            string str_wrapper = str;
            string::iterator it = str_wrapper.begin();
            while (it != str_wrapper.end() && isspace(*it))
                it = str_wrapper.erase(it);
            return str_wrapper;
        }

        LSLIB_API string trim_left(const string& str)
        {
            return trim_left(str.c_str());
        }

        LSLIB_API string trim_right(_lpcstr str)
        {
            string str_wrapper = str;
            string::reverse_iterator it = str_wrapper.rbegin();
            while (it != str_wrapper.rend() && isspace(*it))
                it = string::reverse_iterator(str_wrapper.erase((++it).base()));
            return str_wrapper;
        }

        LSLIB_API string trim_right(const string& str)
        {
            return trim_right(str.c_str());
        }

        LSLIB_API string trim(_lpcstr str, _lchar c)
        {
            string str_wrapper = str;
            str_wrapper = trim_left(str_wrapper.c_str(), c);
            str_wrapper = trim_right(str_wrapper.c_str(), c);
            return str_wrapper;
        }

        LSLIB_API string trim(const string& str, _lchar c)
        {
            return trim(str.c_str(), c);
        }

        LSLIB_API string trim_left(_lpcstr str, _lchar c)
        {
            string str_wrapper = str;
            string::iterator it = str_wrapper.begin();
            while (it != str_wrapper.end() && c == (*it))
                it = str_wrapper.erase(it);
            return str_wrapper;
        }

        LSLIB_API string trim_left(const string& str, _lchar c)
        {
            return trim_left(str.c_str(), c);
        }

        LSLIB_API string trim_right(_lpcstr str, _lchar c)
        {
            string str_wrapper = str;
            string::reverse_iterator it = str_wrapper.rbegin();
            while (it != str_wrapper.rend() && c == (*it))
                it = string::reverse_iterator(str_wrapper.erase((++it).base()));
            return str_wrapper;
        }

        LSLIB_API string trim_right(const string& str, _lchar c)
        {
            return trim_right(str.c_str(), c);
        }

        LSLIB_API string replace(_lpcstr str, _lchar needle, _lchar replacement, size_t index /*= 0*/, int counts /*= -1*/)
        {
            if (needle == replacement)
                return str;

            string str_wrapper = str;
            string::iterator p = str_wrapper.begin();
            size_t replace_index = 0;
            int replace_count = 0;
            while (p != str_wrapper.end())
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
            return str_wrapper;
        }

        LSLIB_API string replace(const string& str, _lchar needle, _lchar replacement, size_t index /*= 0*/, int counts /*= -1*/)
        {
            return replace(str.c_str(), needle, replacement, index, counts);
        }

        LSLIB_API string replace(_lpcstr str, _lpcstr needle, _lpcstr replacement, size_t index /*= 0*/, int counts /*= -1*/)
        {
            if (is_empty(needle))
                return str;
            if (strcmp(needle, replacement) == 0)
                return str;

            size_t needle_len = strlen(needle);
            size_t replacement_len = strlen(replacement);
            int replace_count = 0;

            // calculate new buffer size
            _lpcstr p = str;
            while (*p != 0)
            {
                if (strncmp(p, needle, needle_len) == 0)  replace_count++;
                p++;
            }

            // new buffer
            size_t new_size = strlen(str) - replace_count * needle_len + replace_count * replacement_len;

            _lpstr pbuffer = new _lchar[new_size + 1];
            memset(pbuffer, 0, new_size + 1);

            // process of replacement
            _lpstr insert_point = pbuffer;
            _lpcstr start = str;
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

            string strret = string(pbuffer);
            delete[] pbuffer;
            return strret;
        }

        LSLIB_API string replace(const string& str, const string& needle, const string& replacement, size_t index /*= 0*/, int counts /*= -1*/)
        {
            return replace(str.c_str(), needle.c_str(), replacement.c_str(), index, counts);
        }

        LSLIB_API string format(_lpcstr pfmt, const va_list& args)
        {
#ifdef WIN32
            int len = vsnprintf(NULL, 0, pfmt, args);
            char* pbuf = (char*)malloc(len + 1);
            memset(pbuf, 0, len + 1);
            vsnprintf(pbuf, len, pfmt, args);
#else
            char* pbuf = NULL;
            vasprintf(&pbuf, pfmt, args);
#endif
            string str = pbuf;
            delete[] pbuf;
            return str;
        }

        LSLIB_API string format(_lpcstr pfmt, ...)
        {
            va_list args;
            va_start(args, pfmt);
            string strret = format(pfmt, args);
            va_end(args);
            return strret;
        }

        LSLIB_API string append_format(__out__ string& str, _lpcstr pfmt, ...)
        {
            va_list args;
            va_start(args, pfmt);
            str += format(pfmt, args);
            va_end(args);
            return str;
        }

        LSLIB_API const int split(__out__ string_array& dest, _lpcstr src, _lpcstr patten, bool allow_empty /*= true*/)
        {
            if (is_empty(src)) return 0;
            if (is_empty(patten))
            {
                dest.push_back(src);
                return dest.size();
            }

#define push(str) \
    if (strlen(str) == 0 && !allow_empty); else dest.push_back(str);

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
                    push(pbuff);
                    delete[] pbuff;
                    last_pos = p + strlen(patten);
                    if (last_pos[0] == 0) push(last_pos);
                }
            }
            if (last_pos[0] != 0) push(last_pos);

            return dest.size();
        }

        LSLIB_API const int split(__out__ string_array& dest, const string& src, const string& patten, bool allow_empty /*= true*/)
        {
            return split(dest, src.c_str(), patten.c_str(), allow_empty);
        }

        LSLIB_API _lbyte hex_char_to_byte(_lchar c)
        {
            if (c >= '0' && c <= '9') return c - '0';
            else if (c >= 'a' && c <= 'f') return 10 + c - 'a';
            else if (c >= 'A' && c <= 'F') return 10 + c - 'A';
            else return 0xff;
        }

        LSLIB_API _lchar byte_to_hex_char(_lbyte b)
        {
            if (b >= 0x0 && b <= 0x9) return b + '0';
            else if (b >= 0xa && b <= 0xf) return b - 10 + 'a';
            else return 0;
        }

        LSLIB_API _lbyte_array hex_str_to_byte_array(_lpcstr data)
        {
            assert(!strtool::is_empty(data));

            _lbyte_array arr;
            _lpcstr p = data;
            while (*p != 0)
            {
                _lbyte high = hex_char_to_byte(*p++);
                _lbyte low = hex_char_to_byte(*p++);
                if (high != 0xff && low != 0xff) arr.push_back((high << 4) | low);
            }
            return arr;
        }

        LSLIB_API _lbyte_array hex_str_to_byte_array(const string& data)
        {
            return hex_str_to_byte_array(data.c_str());
        }

        LSLIB_API string byte_array_to_hex_str(_lbyte_array data)
        {
            string str;
            for (size_t i = 0; i < data.size(); i++)
            {
                str += byte_to_hex_char((data[i] & 0xf0) >> 4);
                str += byte_to_hex_char(data[i] & 0x0f);
            }
            return str;
        }

        LSLIB_API string byte_array_to_hex_str(_lbyte data[], int len)
        {
            string str;
            for (size_t i = 0; i < len; i++)
            {
                str += byte_to_hex_char((data[i] & 0xf0) >> 4);
                str += byte_to_hex_char(data[i] & 0x0f);
            }
            return str;
        }

    } // endof strtool

} // end of lslib
