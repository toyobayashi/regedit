#ifndef __REGEDIT_UTIL_HPP__
#define __REGEDIT_UTIL_HPP__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <string>

namespace regedit {

std::string w2a(const std::wstring& wstr);
std::wstring a2w(const std::string& str);
std::string format_message(unsigned long code);
std::string errmsg(long code);
unsigned long long get_time(const FILETIME& ft);

}

#endif
