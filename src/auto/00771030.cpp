#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the text-entry range held at this+0x8/0x0C and frees the block at this+0x8, then clears the three slots.
CSF_FUNC(int, __cdecl, TextDatabase_ReleaseEntryRange, 0x004F01A0, (int firstEntry, int endEntry, int, int));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00871914_DestroyBaseLayer(C_00871914* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00871914);

    int result = CSF_AT(int, self, 0x8);
    if (result != 0) {
        // The original's fourth argument is an uninitialized stack slot; pass 0.
        const int uninitializedArg = 0;
        TextDatabase_ReleaseEntryRange(result,
                                       CSF_AT(int, self, 0xC),
                                       reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4),
                                       uninitializedArg);

        // The original reloads the handle after the release call.
        result = CSF_AT(int, self, 0x8);
        if (result != 0) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, self, 0x8)));
        }
    }

    CSF_AT(int, self, 0x8) = 0;
    CSF_AT(int, self, 0xC) = 0;
    CSF_AT(int, self, 0x10) = 0;
    return result;
}

HOOK(0x00771030, C_00871914_DestroyBaseLayer, "auto");
