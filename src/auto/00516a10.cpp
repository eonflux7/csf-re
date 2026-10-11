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

// The disassembly pushes four cdecl arguments before the call (the caller pops them with add esp, 0x10).
CSF_FUNC(int, __cdecl, CloseSaveBufferHandle, 0x00515080, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the save buffer owned by C_00840af8 (CProfileManager / CSaveGameManager) and clears its state.
static int __fastcall C_00840af8_ReleaseBufferAndClear(C_00840af8* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840AF8);

    int result = static_cast<int>(self->field_0008);
    if (result != 0) {
        CloseSaveBufferHandle(result,
                              static_cast<int>(self->field_000c),
                              reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4),
                              reinterpret_cast<int>(self));

        // The original reloads the buffer pointer after the close call.
        result = static_cast<int>(self->field_0008);
        if (result != 0) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
        }
    }

    self->field_0008 = 0;
    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    return result;
}

HOOK(0x00516A10, C_00840af8_ReleaseBufferAndClear, "auto");
