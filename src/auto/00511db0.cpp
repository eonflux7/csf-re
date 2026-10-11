#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees (bound by address).
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteFloat, 0x00695330, (void* self, void* edx, float a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup, 0x006951D0, (void* self, void* edx));

// Key-name strings used as group/field labels.
CSF_GLOBAL(const char*, g_pszKey_DATE, 0x00899BA4);
CSF_GLOBAL(const char*, g_pszKey_YEAR, 0x00899BA8);
CSF_GLOBAL(const char*, g_pszKey_MONTH, 0x00899BAC);
CSF_GLOBAL(const char*, g_pszKey_DAY, 0x00899BB0);
CSF_GLOBAL(const char*, g_pszKey_HOUR, 0x00899BB4);
CSF_GLOBAL(const char*, g_pszKey_MINUTE, 0x00899BB8);
CSF_GLOBAL(const char*, g_pszKey_SECOND, 0x00899BBC);

// Writes a "DATE" group containing year/month/day/hour/minute (int) and second (float)
// from a 24-byte date record at dateRecord, then closes the group and returns its result.
static int __cdecl SaveGame_WriteDateGroup(S_00694cd0* writer, int dateRecord)
{
    const int* date = reinterpret_cast<const int*>(dateRecord);

    CoreZlibLog_BeginGroup(writer, nullptr, const_cast<char*>(g_pszKey_DATE), 0, 0);
    CoreZlibLog_WriteInteger(writer, nullptr, date[0], const_cast<char*>(g_pszKey_YEAR));
    CoreZlibLog_WriteInteger(writer, nullptr, date[1], const_cast<char*>(g_pszKey_MONTH));
    CoreZlibLog_WriteInteger(writer, nullptr, date[2], const_cast<char*>(g_pszKey_DAY));
    CoreZlibLog_WriteInteger(writer, nullptr, date[3], const_cast<char*>(g_pszKey_HOUR));
    CoreZlibLog_WriteInteger(writer, nullptr, date[4], const_cast<char*>(g_pszKey_MINUTE));
    CoreZlibLog_WriteFloat(writer, nullptr, *reinterpret_cast<const float*>(dateRecord + 20),
                           const_cast<char*>(g_pszKey_SECOND));

    // Tail jump in the original: result of the end-group call is returned as-is.
    return static_cast<int>(CoreZlibLog_EndGroup(writer, nullptr));
}

HOOK(0x511DB0, SaveGame_WriteDateGroup, "auto");
