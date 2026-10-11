#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_DATE, 0x00899BA4);
CSF_GLOBAL(const char*, g_pszKey_YEAR, 0x00899BA8);
CSF_GLOBAL(const char*, g_pszKey_MONTH, 0x00899BAC);
CSF_GLOBAL(const char*, g_pszKey_DAY, 0x00899BB0);
CSF_GLOBAL(const char*, g_pszKey_HOUR, 0x00899BB4);
CSF_GLOBAL(const char*, g_pszKey_MINUTE, 0x00899BB8);
CSF_GLOBAL(const char*, g_pszKey_SECOND, 0x00899BBC);

// Reads the save-game date block (year, month, day, hour, minute, seconds)
// from a property object into an output array of six 4-byte values.
static char __cdecl SaveGames_ReadDateTimeProperties(S_00692b80* a1, int a2)
{
    if (!a1)
        return 0;

    std::uint8_t* dateProperty = FindProperty(reinterpret_cast<std::uint8_t*>(a1), nullptr,
                                              const_cast<char*>(g_pszKey_DATE));
    std::uint8_t* propertyObject = dateProperty;
    if (!propertyObject)
        return 0;

    int* out = reinterpret_cast<int*>(a2);
    out[0] = ReadIntegerProperty(propertyObject, nullptr, const_cast<char*>(g_pszKey_YEAR));
    out[1] = ReadIntegerProperty(propertyObject, nullptr, const_cast<char*>(g_pszKey_MONTH));
    out[2] = ReadIntegerProperty(propertyObject, nullptr, const_cast<char*>(g_pszKey_DAY));
    out[3] = ReadIntegerProperty(propertyObject, nullptr, const_cast<char*>(g_pszKey_HOUR));
    out[4] = ReadIntegerProperty(propertyObject, nullptr, const_cast<char*>(g_pszKey_MINUTE));
    reinterpret_cast<float*>(out)[5] =
        static_cast<float>(ReadFloatProperty(propertyObject, nullptr, const_cast<char*>(g_pszKey_SECOND)));
    return 1;
}

HOOK(0x511D10, SaveGames_ReadDateTimeProperties, "auto");
