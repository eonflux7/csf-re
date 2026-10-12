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

CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __stdcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (int a1));

// Sets the active flag at +0x3DC. Turning it on from off triggers the
// menu mission info setup for index 90. Always clears the counter at +0x224.
static char __fastcall SetMenuMissionActiveFlag(void* self, void* /*edx*/, char activeFlag)
{
    char result = 0;

    if (activeFlag)
    {
        result = CSF_AT(uint8_t, self, 0x3DC);
        if (!result)
        {
            Get_008b0678();
            result = (char)MenuMissionInfo_SetGlobalIndex(90);
        }
    }

    CSF_AT(uint8_t, self, 0x3DC) = (uint8_t)activeFlag;
    CSF_AT(int, self, 0x224) = 0;
    return result;
}

HOOK(0x004951B0, SetMenuMissionActiveFlag, "auto");
