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

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the 30-entry table of 48-byte records that starts at this+0x5C0 and runs
// downward. Records whose tag is >= 8 and whose pointer field is non-null have that
// block freed; every record is then reset to tag 7, zero dword, zero pointer low word.
static char __fastcall C_00867900_ClearPointerTable(C_00867900* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0))
        SceneLoader_NoOp();
    CSF_AT(uint8_t, self, 0) = 0;

    uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + 0x5C0;
    uint32_t lastValue = 0;
    for (int remaining = 30; remaining != 0; --remaining)
    {
        cursor -= 0x30;
        lastValue = CSF_AT(uint32_t, cursor, 0);
        if (lastValue >= 8)
        {
            uint8_t* blockField = cursor - 0x14;
            lastValue = CSF_AT(uint32_t, blockField, 0);
            if (lastValue != 0)
            {
                int freeResult = Mem_FreeDebug(reinterpret_cast<void*>(lastValue));
                lastValue = (lastValue & 0xFFFFFF00u) | (static_cast<uint32_t>(freeResult) & 0xFFu);
            }
        }
        CSF_AT(uint32_t, cursor, 0) = 7;
        CSF_AT(uint32_t, cursor, -4) = 0;
        CSF_AT(uint16_t, cursor - 0x14, 0) = 0;
    }
    return static_cast<char>(lastValue & 0xFFu);
}

HOOK(0x0073D500, C_00867900_ClearPointerTable, "auto");
