#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(double, __cdecl, _atof, 0x007B04AF, (const char* text));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Reads a numeric field: a component-backed value when kind == 0, otherwise
// a length-prefixed text value (kind 3 or 4) converted with atof.
static double __fastcall CoreParser_ReadNumericField(int* self, void* /*edx*/)
{
    int kind = self[0];
    if (kind == 0) {
        std::uint32_t component = CSF_AT(std::uint32_t, self, 0x18);
        return ReadComponentFloat(component);
    }
    if (kind != 3 && kind != 4) {
        return static_cast<double>(g_fZero);
    }

    // Descriptor: [0] = text length, [4] = text pointer.
    std::uint32_t* desc = reinterpret_cast<std::uint32_t*>(CSF_AT(std::uint32_t, self, 4));
    std::uint32_t length = desc[0];
    if (length > 0xFFu) {
        length = 255;
    }

    char text[256];
    std::memcpy(text, reinterpret_cast<const void*>(desc[1]), length);
    text[length] = 0;
    return _atof(text);
}

HOOK(0x692880, CoreParser_ReadNumericField, "auto");
