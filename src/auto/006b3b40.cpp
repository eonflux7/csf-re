#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Visitor callback type for RpClumpForAllAtomics: __cdecl (atomic, data) -> int.
using AtomicVisitorFn = int(__cdecl*)(int atomic, int data);

// Original visitor passed to RpClumpForAllAtomics (body not in packet; bound by address only).
CSF_FUNC(int, __cdecl, GfxObject_CollectAtomic, 0x006B3230, (int atomic, int data));

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RpClumpGetNumAtomics, 0x0058F560, (int a1));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Rebuilds the per-atomic pointer table owned by this object:
// counts the atomics in the clump, frees the old table, allocates a new one
// and fills it by visiting every atomic of the clump.
static int __fastcall C_00860288_RebuildAtomicTable(C_00860288* self, void* /*edx*/)
{
    self->field_001c = static_cast<uint16_t>(RpClumpGetNumAtomics(static_cast<int>(self->field_000c)));

    if (self->field_0018) {
        Mem_Free(reinterpret_cast<void*>(self->field_0018));
        self->field_0018 = 0;
    }

    uint32_t tableBlock = reinterpret_cast<uint32_t>(
        Mem_Alloc(static_cast<std::size_t>(self->field_001c) * 4));
    self->field_0018 = tableBlock;

    // Visitor state: [0] table, [1] index counter, [2] owner object, [3] extra field.
    uint32_t visitState[4];
    visitState[0] = tableBlock;
    visitState[1] = 0;
    visitState[2] = reinterpret_cast<uint32_t>(self);
    visitState[3] = self->field_0014;

    return RpClumpForAllAtomics(
        static_cast<int>(self->field_000c),
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006B3230)),
        reinterpret_cast<int>(&visitState[0]));
}

HOOK(0x006B3B40, C_00860288_RebuildAtomicTable, "auto");
