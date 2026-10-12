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

// Callee: __fastcall(ecx = self, stack: value, out pointer) -> ret 8
CSF_FUNC(int, __fastcall, sub_519EC0, 0x00519EC0, (void* self, void* edx, int a2, uint32_t* a3));
// Callee: __thiscall(ecx = self, stack: sectorIndex) -> ret 4
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorRecord, 0x00762470, (void* self, void* , int sectorIndex));

static int __fastcall C_008406e8_ActivateSector(C_008406e8* self, void* /*edx*/, int a2)
{
    // Stack slot [ebp-8]: 8 bytes of room for the callee's out value.
    alignas(8) uint8_t scratchBytes[8];
    uint32_t* outRecord = reinterpret_cast<uint32_t*>(scratchBytes);

    // Sector table: element a2 is at table_base + a2 * 556 (0x22C).
    uint32_t tableBase = CSF_AT(uint32_t, self, 0x08);
    uint32_t elementAddress = CSF_AT(uint32_t, reinterpret_cast<void*>(tableBase), 4) + static_cast<uint32_t>(a2) * 556u;

    sub_519EC0(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr, static_cast<int>(elementAddress), outRecord);

    return SceneLoader_GetSectorRecord(self, nullptr, a2);
}

HOOK(0x0051B9E0, C_008406e8_ActivateSector, "auto");
