#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using Char = char;
using String = legacy::vc71::string<Char>;
CSF_FUNC(void*, __fastcall, Grow, 0x00410130, (String* self, void* edx, uint32_t size, uint32_t length));
CSF_FUNC(String*, __fastcall, Range, 0x0043EA30, (String* self, void* edx, String* source, uint32_t pos, uint32_t count));
struct Operations {
    using char_type = Char;
    static __forceinline void grow(String* self, uint32_t size, uint32_t length) { Grow(self, nullptr, size, length); }
    static __forceinline String* append(String* self, String* source, uint32_t pos, uint32_t count) { return Range(self, nullptr, source, pos, count); }
};
static String* __fastcall StlString_0043eb30(String* self, void*, const Char* source, uint32_t count) {
    return legacy::vc71::string_append_ptr<Operations>(self, source, count);
}
HOOK(0x0043EB30, StlString_0043eb30, "stl");
