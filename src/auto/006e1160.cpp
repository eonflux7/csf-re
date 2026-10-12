#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, sub_6DE360, 0x006DE360, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0085e65c_InitVtable, 0x006923A0, (C_0085e65c* self, void*));

// Releases the owned sub-object and the owned buffer, resets the buffer
// bookkeeping to the empty state, then re-initializes the base part.
static void __fastcall C_00861a28_ReleaseAndReset(C_00861a28* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00861A28);

    S_006dcf90* ownedSubobject = reinterpret_cast<S_006dcf90*>(self->field_0024);
    if (ownedSubobject)
    {
        sub_6DE360(ownedSubobject, nullptr);
        Mem_Free(ownedSubobject);
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

HOOK(0x006E1160, C_00861a28_ReleaseAndReset, "auto");
