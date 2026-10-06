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

}  // namespace legacy
