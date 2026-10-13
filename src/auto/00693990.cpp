#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Looks up a named property on this object and, if found, reads its float value.
// When the property is missing, returns the zero constant stored at g_fZero.
static double __fastcall S_00692b80_ReadFloatAttribute(S_00692b80* self, void* /*edx*/, char* String1)
{
    std::uint8_t* property = FindProperty(reinterpret_cast<std::uint8_t*>(self), nullptr, String1);
    float zeroValue = g_fZero;
    if (property != nullptr)
    {
        return ReadComponentFloat(reinterpret_cast<std::uint32_t>(property));
    }
    return static_cast<double>(zeroValue);
}

HOOK(0x00693990, S_00692b80_ReadFloatAttribute, "auto");
