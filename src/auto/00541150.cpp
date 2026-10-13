#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using Char = uint16_t;
using String = legacy::vc71::string<Char>;
CSF_FUNC(void*, __fastcall, Grow, 0x00486550, (String* self, void* edx, uint32_t size, uint32_t length));
struct Operations {
    using char_type = Char;
    static __forceinline void grow(String* self, uint32_t size, uint32_t length) { Grow(self, nullptr, size, length); }
};
static String* __fastcall StlString_00541150(String* self, void*, uint32_t count, Char value) {
    return legacy::vc71::string_append_fill<Operations>(self, count, value);
}
HOOK(0x00541150, StlString_00541150, "stl");
