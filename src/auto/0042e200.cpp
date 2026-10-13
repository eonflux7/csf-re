#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using Char = char;
using String = legacy::vc71::string<Char>;
struct Operations {
    using char_type = Char;
    static __forceinline int compare(const Char* a, const Char* b, uint32_t count) {
        for (uint32_t i = 0; i < count; ++i) {
            auto x = static_cast<uint8_t>(a[i]);
            auto y = static_cast<uint8_t>(b[i]);
            if (x != y) return x < y ? -1 : 1;
        }
        return 0;
    }
};
static int __fastcall StlString_0042e200(String* self, void*, uint32_t pos, uint32_t count, const Char* source, uint32_t source_count) {
    return legacy::vc71::string_compare<Operations>(self, pos, count, source, source_count);
}
HOOK(0x0042E200, StlString_0042e200, "stl");
