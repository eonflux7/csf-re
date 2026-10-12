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

// Element stride of the record array is 16 bytes; field_0004 = begin, field_0008 = end.
CSF_FUNC(int*, __fastcall, BddTables_TruncateRecordVector, 0x0051A730,
         (void* self, void* edx, int* a2, int a3, int a4));
CSF_FUNC(uint32_t, __fastcall, BddTables_GrowRecordVector, 0x0051A7D0,
         (void* self, void* edx, uint32_t endPtr, uint32_t count, char* fillValue));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// The vtable of S_0051a730 that the original references next to its call to 0x51A730.
[[maybe_unused]] static const auto kS_0051a730Vtable = CSF_VTBL(0x0051A730);

// Resizes the 16-byte-element record vector of a bdd table to a2 elements:
// shrinks by truncating from index a2, grows by calling the grow helper with the
// value a3 as fill. Then frees Block (if any) with the debug free and returns its result.
static void* __fastcall BddTables_ResizeRecordVectorAndFreeBlock(S_0051a730* self, void* /*edx*/,
                                                                 unsigned int a2, char a3, void* Block,
                                                                 int a5, int a6)
{
    uint32_t begin = self->field_0004;

    uint32_t elementCount = 0;
    if (begin) {
        elementCount = static_cast<uint32_t>(static_cast<int>(self->field_0008 - begin) >> 4);
    }

    if (elementCount >= a2) {
        if (begin && a2 < elementCount) {
            BddTables_TruncateRecordVector(self, nullptr,
                                           reinterpret_cast<int*>(&a2),
                                           static_cast<int>(begin + (a2 << 4)),
                                           static_cast<int>(self->field_0008));
        }
    } else {
        uint32_t currentCount = 0;
        if (begin) {
            currentCount = static_cast<uint32_t>(static_cast<int>(self->field_0008 - begin) >> 4);
        }
        BddTables_GrowRecordVector(self, nullptr,
                                   self->field_0008,
                                   a2 - currentCount,
                                   &a3);
    }

    if (Block) {
        return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(Mem_FreeDebug(Block))));
    }
    return Block;
}

HOOK(0x0051AAC0, BddTables_ResizeRecordVectorAndFreeBlock, "auto");
