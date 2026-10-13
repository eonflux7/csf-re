#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082daf4_ResetEntries, 0x0040B5B0, (C_0082daf4* self, void*));
CSF_FUNC(int, __fastcall, C_0082daf4_ResetSecondaryEntries, 0x0040B690, (C_0082daf4* self, void*));
CSF_FUNC(int, __cdecl, EntryTable_FreePointerRange, 0x0040B480, (int first, int end, int, int));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_0082daf4_ReleaseOwnedBuffers(C_0082daf4* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0082DAF4);

    C_0082daf4_ResetEntries(self, nullptr);
    C_0082daf4_ResetSecondaryEntries(self, nullptr);

    // First owned buffer: this+0x20, range descriptor at this+0x1C.
    uint32_t firstBuffer = self->field_0020;
    if (firstBuffer != 0) {
        EntryTable_FreePointerRange(
            (int)firstBuffer,
            (int)CSF_AT(uint32_t, self, 0x24),
            (int)((uint8_t*)self + 0x1C),
            0);
        if (self->field_0020 != 0)
            Mem_FreeDebug((void*)self->field_0020);
    }
    self->field_0020 = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;

    // Second owned buffer: this+0x10, range descriptor at this+0x0C.
    int result = (int)self->field_0010;
    if (result != 0) {
        EntryTable_FreePointerRange(
            result,
            (int)CSF_AT(uint32_t, self, 0x14),
            (int)((uint8_t*)self + 0x0C),
            0);
        result = (int)self->field_0010;
        if (result != 0)
            result = Mem_FreeDebug((void*)self->field_0010);
    }
    self->field_0010 = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;

    self->vtbl = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0040BCB0, C_0082daf4_ReleaseOwnedBuffers, "auto");
