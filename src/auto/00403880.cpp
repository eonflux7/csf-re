#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_string.h"
using Char = char;
using String = legacy::vc71::string<Char>;
static String* __fastcall StlString_00403880(String* self, void*, uint32_t pos, uint32_t count) {
    return legacy::vc71::string_erase<Char>(self, pos, count);
}
HOOK(0x00403880, StlString_00403880, "stl");
