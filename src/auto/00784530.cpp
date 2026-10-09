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

CSF_FUNC(int, __cdecl, WeaponDefs_ReleaseRecordEntries, 0x00783670, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the record block owned by a CWeaponDefTable: frees the record array
// (after releasing its entries), then the owner block itself, and clears the links.
static void __fastcall CWeaponDefTable_ReleaseRecordBlock(CWeaponDefTable* self, void* /*edx*/)
{
    uint32_t ownerBlockAddr = self->m_pRecords;
    self->vtbl = CSF_VTBL(0x008728A0);

    if (ownerBlockAddr != 0) {
        void* ownerBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(ownerBlockAddr));
        uint32_t recordArray = CSF_AT(uint32_t, ownerBlock, 0x4);

        if (recordArray != 0) {
            uint32_t recordCount = CSF_AT(uint32_t, ownerBlock, 0x8);
            WeaponDefs_ReleaseRecordEntries(static_cast<int>(recordArray),
                                            static_cast<int>(recordCount),
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
        self->m_pRecords = 0;
    }
}

HOOK(0x00784530, CWeaponDefTable_ReleaseRecordBlock, "auto");
