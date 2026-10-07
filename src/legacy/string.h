// MSVC 7.1 (Dinkumware) std::basic_string<char> layout, for objects shared with the original game.
#pragma once
#include <cstddef>
#include <cstdint>

namespace legacy {

struct string {
    uint32_t alloc;  // empty allocator object, still occupies 4 bytes in VC7.1
    union {
        char buf[16];
        char* ptr;
    } bx;
    uint32_t size;
    uint32_t res;  // capacity; < 16 means inline buffer

    const char* c_str() const { return res < 16 ? bx.buf : bx.ptr; }
};
static_assert(sizeof(string) == 28);
static_assert(offsetof(string, size) == 20 && offsetof(string, res) == 24);

// std::basic_string<wchar_t> (VC7.1): same 28 bytes, 8-wchar inline buffer (capacity starts at 7).
struct wstring {
    uint32_t alloc;
    union {
        wchar_t buf[8];
        wchar_t* ptr;
    } bx;
    uint32_t size;
    uint32_t res;  // capacity; < 8 means inline buffer

    const wchar_t* c_str() const { return res < 8 ? bx.buf : bx.ptr; }
};
static_assert(sizeof(wstring) == 28);
static_assert(offsetof(wstring, size) == 20 && offsetof(wstring, res) == 24);

}  // namespace legacy
