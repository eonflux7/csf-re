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

CSF_FUNC(int, __cdecl, PerfCounters_ReleaseBaseEntries, 0x0078BE10, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the base-state block owned by a C_00872abc: frees the entry array
// (after releasing its entries), then the owner block itself, and clears the links.
static void __fastcall C_00872abc_ReleaseBaseState(C_00872abc* self, void* /*edx*/)
{
    uint32_t ownerBlockAddr = self->field_0004;
    self->vtbl = CSF_VTBL(0x00872ABC);

    if (ownerBlockAddr != 0) {
        void* ownerBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerBlockAddr));
        uint32_t entryArray = CSF_AT(uint32_t, ownerBlock, 0x4);

        if (entryArray != 0) {
            uint32_t entryCount = CSF_AT(uint32_t, ownerBlock, 0x8);
            PerfCounters_ReleaseBaseEntries(static_cast<int>(entryArray),
                                            static_cast<int>(entryCount),
                                            static_cast<int>(ownerBlockAddr),
                                            0);
            if (CSF_AT(uint32_t, ownerBlock, 0x4) != 0) {
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, ownerBlock, 0x4))));
            }
        }

        CSF_AT(uint32_t, ownerBlock, 0x4) = 0;
        CSF_AT(uint32_t, ownerBlock, 0x8) = 0;
        CSF_AT(uint32_t, ownerBlock, 0xC) = 0;
        Mem_Free(ownerBlock);
        self->field_0004 = 0;
    }
}

HOOK(0x0078C8E0, C_00872abc_ReleaseBaseState, "auto");
