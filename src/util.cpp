#include "util.hpp"
#include <string>
#include <cstring>

#define EPOCHFILETIME (116444736000000000UL)

namespace regedit {

std::wstring a2w(const std::string& str) {
  int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
  if (len == 0) return L"";
  wchar_t* buf = new wchar_t[len];
  memset(buf, 0, len * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf, len);
  std::wstring res(buf);
  delete[] buf;
  return res;
}

std::string w2a(const std::wstring& wstr) {
  int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
  if (len == 0) return "";
  char* buf = new char[len];
  memset(buf, 0, len * sizeof(char));
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buf, len, nullptr, nullptr);
  std::string res(buf);
  delete[] buf;
  return res;
}

std::string format_message(DWORD code) {
  LPVOID buf;
  if (FormatMessageW(
    FORMAT_MESSAGE_FROM_SYSTEM
    | FORMAT_MESSAGE_IGNORE_INSERTS
    | FORMAT_MESSAGE_ALLOCATE_BUFFER,
    NULL,
    code,
    // MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
    GetSystemDefaultLangID(),
    (LPWSTR) &buf,
    0,
    NULL
  )) {
    std::wstring msg = (wchar_t*)buf;
    LocalFree(buf);
    size_t pos = msg.find_last_of(L"\r\n");
    msg = msg.substr(0, pos - 1);
    std::string utf8str = w2a(msg);

    return utf8str;
  } else {
    char buf[10] = { 0 };
    _itoa(GetLastError(), buf, 10);
    return std::string("Cannot format message. Win32 error code: ") + buf;
  }
}

ULONGLONG get_time(const FILETIME& ft) {
  ULARGE_INTEGER li;
  ULONGLONG tt = 0;
  li.LowPart = ft.dwLowDateTime;
  li.HighPart = ft.dwHighDateTime;
  tt = (li.QuadPart - EPOCHFILETIME) / 10 / 1000;
  return tt;
}

}
