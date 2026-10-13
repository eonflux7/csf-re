#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

static double __fastcall S_00692b80_ReadFloatProperty(
    S_00692b80* self, void* /*edx*/, char* keyword)
{
    auto* property = FindProperty(
        reinterpret_cast<std::uint8_t*>(self), nullptr, keyword);
    if (property)
        return ReadComponentFloat(reinterpret_cast<std::uint32_t>(property));

    return 0.0;
}

HOOK(0x00693940, S_00692b80_ReadFloatProperty, "auto");
