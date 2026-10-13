#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using Char = uint16_t;
using String = legacy::vc71::string<Char>;
CSF_FUNC(int, __cdecl, Compare, 0x005157F0, (const Char* a, const Char* b, uint32_t count));
struct Operations {
    using char_type = Char;
    static __forceinline int compare(const Char* a, const Char* b, uint32_t count) { return Compare(a, b, count); }
};
static int __fastcall StlString_005158f0(String* self, void*, uint32_t pos, uint32_t count, const Char* source, uint32_t source_count) {
    return legacy::vc71::string_compare<Operations>(self, pos, count, source, source_count);
}
HOOK(0x005158F0, StlString_005158f0, "stl");
