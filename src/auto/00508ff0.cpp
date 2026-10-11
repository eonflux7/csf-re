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

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(void*, __fastcall, sub_69D070, 0x0069D070, (void* self, void* edx, char* a2, void* a3));
CSF_FUNC(char, __fastcall, sub_69D1B0, 0x0069D1B0, (S_0069d0f0* self, void* edx, const char* lpValueName, uint8_t* lpData));
CSF_FUNC(char, __fastcall, sub_69CFA0, 0x0069CFA0, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Reads the "Language" value from the app config and returns it (0 if absent).
static int __cdecl AppMain_ReadLanguageId()
{
    int languageId = 0;

    uint8_t* globalBase = reinterpret_cast<uint8_t*>(GetGlobalValue_75FDF0());
    uint32_t slotCount = CSF_AT(uint32_t, globalBase, 0xEC);
    char* slotPtr;
    if (slotCount < 0x10)
        slotPtr = reinterpret_cast<char*>(globalBase + 0xD4 + 4);
    else
        slotPtr = reinterpret_cast<char*>(CSF_AT(uint32_t, globalBase, 0xD4 + 4));

    alignas(8) uint8_t stateBytes[0x40];
    S_0069d0f0& valueKey = *reinterpret_cast<S_0069d0f0*>(stateBytes);
    sub_69D070(&valueKey, nullptr, slotPtr, &g_byte_82E65C);

    alignas(8) uint8_t dataBuf[0x10C];
    if (sub_69D1B0(&valueKey, nullptr, CSF_STR(0x0083F8E0, "Language"), dataBuf))
        languageId = *reinterpret_cast<int*>(dataBuf);

    sub_69CFA0(&valueKey, nullptr);
    return languageId;
}

HOOK(0x508FF0, AppMain_ReadLanguageId, "auto");
