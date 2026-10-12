#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Hud_IsStringEqualIgnoreCase, 0x0042D5F0, (char* String1, char* String2));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static bool __fastcall GfxCollisionShapes_AreShapesEqual(
    char* self, void* /*edx*/, char* other)
{
    const std::uint32_t otherType = CSF_AT(std::uint32_t, other, 0);
    const std::uint32_t shapeType = CSF_AT(std::uint32_t, self, 0);
    if (shapeType != otherType)
        return false;

    std::uint8_t matches = 1;
    switch (shapeType)
    {
    case 1:
    case 2:
    case 4:
    {
        const char* otherName =
            CSF_AT(std::uint32_t, other, 0x38) < 0x10u
                ? other + 0x24
                : CSF_AT(const char*, other, 0x24);
        const char* selfName =
            CSF_AT(std::uint32_t, self, 0x38) < 0x10u
                ? self + 0x24
                : CSF_AT(const char*, self, 0x24);

        matches &= selfName && otherName &&
                   __stricmp(selfName, otherName) == 0;
        matches &= CSF_AT(float, other, 0x54) ==
                   CSF_AT(float, self, 0x54);
        break;
    }
    case 3:
    {
        char* otherName =
            CSF_AT(std::uint32_t, other, 0x1C) < 0x10u
                ? other + 8
                : CSF_AT(char*, other, 8);
        char* selfName =
            CSF_AT(std::uint32_t, self, 0x1C) < 0x10u
                ? self + 8
                : CSF_AT(char*, self, 8);

        matches = static_cast<std::uint8_t>(
            Hud_IsStringEqualIgnoreCase(selfName, otherName));
        break;
    }
    default:
        return true;
    }

    matches &= CSF_AT(float, other, 0x58) ==
               CSF_AT(float, self, 0x58);
    matches &= CSF_AT(float, other, 0x5C) ==
               CSF_AT(float, self, 0x5C);
    return (matches & 1) != 0;
}

HOOK(0x006B2620, GfxCollisionShapes_AreShapesEqual, "auto");
