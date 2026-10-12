#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, ClearRecordStorage_6B59F0, 0x006B59F0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0085e65c_InitVtable, 0x006923A0, (C_0085e65c* self, void*));

// Vtable of the record-storage object cleared below (referenced by the original's call site).
static void* const kRecordStorageVtable = CSF_VTBL(0x006B59F0);

// Clears the owned record storage and frees it, resets the buffer bookkeeping to the
// empty state, then re-initializes the base part.
static void __fastcall C_00861a24_ReleaseAndReset(C_00861a24* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00861A24);

    void* recordStorage = reinterpret_cast<void*>(self->field_0024);
    if (recordStorage)
    {
        ClearRecordStorage_6B59F0(recordStorage, nullptr);
        Mem_Free(recordStorage);
        self->field_0024 = 0;
    }

    // Capacity is unsigned: only free the heap buffer when it is past the inline threshold.
    if (self->field_0020 >= 0x10 && self->field_000c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_000c));

    self->field_0020 = 15;
    self->field_001c = 0;
    self->field_000c &= 0xFFFFFF00u; // LOBYTE(field_000c) = 0
    C_0085e65c_InitVtable(reinterpret_cast<C_0085e65c*>(self), nullptr);
}

HOOK(0x006E2C10, C_00861a24_ReleaseAndReset, "auto");
