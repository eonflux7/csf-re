#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using Char = char;
using String = legacy::vc71::string<Char>;
CSF_FUNC(void*, __fastcall, Grow, 0x00410130, (String* self, void* edx, uint32_t size, uint32_t length));
CSF_FUNC(String*, __fastcall, Erase, 0x00403880, (String* self, void* edx, uint32_t pos, uint32_t count));
struct Operations {
    using char_type = Char;
    static __forceinline void grow(String* self, uint32_t size, uint32_t length) { Grow(self, nullptr, size, length); }
    static __forceinline void erase(String* self, uint32_t pos, uint32_t count) { Erase(self, nullptr, pos, count); }
};
static String* __fastcall StlString_004109e0(String* self, void*, String* source, uint32_t pos, uint32_t count) {
    return legacy::vc71::string_assign_range<Operations>(self, source, pos, count);
}
HOOK(0x004109E0, StlString_004109e0, "stl");
