#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using Char = uint16_t;
using String = legacy::vc71::string<Char>;
static String* __fastcall StlString_004856e0(String* self, void*, uint32_t pos, uint32_t count) {
    return legacy::vc71::string_erase<Char>(self, pos, count);
}
HOOK(0x004856E0, StlString_004856e0, "stl");
