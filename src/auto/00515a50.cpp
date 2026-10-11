#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup, 0x00695260, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));

CSF_GLOBAL(const char*, g_pszKey_NOMBRE_profiles_savegames, 0x00899BC8);

// Writes every uint16 element of a word array (inline when capacity < 8, otherwise
// through the heap pointer stored at +4) to the save-game log, as one log group.
static int __stdcall Profiles_WriteWordArrayToLog(S_00694cd0* a1, int a2)
{
    CoreZlibLog_BeginGroup(a1, nullptr, const_cast<char*>(g_pszKey_NOMBRE_profiles_savegames), 1, 1);

    uint32_t count = CSF_AT(uint32_t, a2, 0x14);
    uint32_t index = 0;
    if (count != 0)
    {
        for (;;)
        {
            uint32_t capacity = CSF_AT(uint32_t, a2, 0x18);
            uint8_t* data = reinterpret_cast<uint8_t*>(a2) + 4;
            if (capacity >= 8)
                data = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, a2, 4)));

            uint16_t word = reinterpret_cast<uint16_t*>(data)[index];
            index++;
            CoreZlibLog_WriteInteger(a1, nullptr, static_cast<int>(word), nullptr);

            if (index >= count)
                break;
        }
    }

    return static_cast<int>(CoreZlibLog_EndGroup(a1, nullptr));
}

HOOK(0x515A50, Profiles_WriteWordArrayToLog, "auto");
