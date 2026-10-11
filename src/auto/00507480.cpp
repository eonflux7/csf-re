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

// Callees, bound by address with the signatures from the packet.
CSF_FUNC(void, __cdecl, sub_69CF60, 0x0069CF60, ());
CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the mission-script element range owned by this object (offsets 0x28..0x34),
// destroying the elements and freeing the block at 0x28 if one is present.
static int __fastcall ReleaseMissionScriptElementRange(int* self, void* /*edx*/)
{
    sub_69CF60();

    int result = CSF_AT(int, self, 0x28);
    if (result)
    {
        MissionScript_DestroyElementRange(
            reinterpret_cast<int*>(CSF_AT(int, self, 0x28)),
            reinterpret_cast<int*>(CSF_AT(int, self, 0x2C)));

        result = CSF_AT(int, self, 0x28);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, self, 0x28)));
    }

    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;
    CSF_AT(int, self, 0x30) = 0;
    return result;
}

HOOK(0x507480, ReleaseMissionScriptElementRange, "auto");
