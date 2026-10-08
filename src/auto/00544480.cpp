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

// Callee 0x544340: the caller pushes four arguments and cleans up with add esp,0x10.
// Argument order (first to last): [a1+4], [a1+8], a1, a1.
CSF_FUNC(int, __cdecl, MenuSaveGame_ClearSlotRange, 0x00544340,
         (int* first, int* second, int* self_a, int* self_b));

// Game heap free used by the original (Mem_FreeDebug).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the slot's owned pointer pair (+4 buffer, +8 secondary) and its counter (+12),
// releasing the +4 buffer through the children-release helper first.
static int* __cdecl MenuSaveGame_ClearSlotState(int* a1)
{
    int* result = CSF_AT(int*, a1, 0x4);
    if (result)
    {
        int* secondary = CSF_AT(int*, a1, 0x8);
        MenuSaveGame_ClearSlotRange(result, secondary, a1, a1);

        result = CSF_AT(int*, a1, 0x4);
        if (result)
        {
            result = reinterpret_cast<int*>(
                static_cast<intptr_t>(Mem_FreeDebug(CSF_AT(void*, a1, 0x4))));
        }
    }

    CSF_AT(uint32_t, a1, 0x4) = 0;
    CSF_AT(uint32_t, a1, 0x8) = 0;
    CSF_AT(uint32_t, a1, 0xC) = 0;
    return result;
}

HOOK(0x00544480, MenuSaveGame_ClearSlotState, "auto");
