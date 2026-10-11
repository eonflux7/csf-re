#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(unsigned int, __fastcall, ResetDerivedState, 0x00408A40, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ReleaseEmbeddedMembers, 0x00406030, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_0082d5e0: resets its vtables, releases two owned
// blocks, tears down the embedded sub-object at +0x0C, then leaves the
// object as its C_0082d410 base.
static void __fastcall C_0082d5e0_Destroy(C_0082d5e0* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x0082D5E0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D5C4));

    ResetDerivedState(self, nullptr);

    if (self->field_002c) {
        Mem_Free(reinterpret_cast<void*>(self->field_002c));
        self->field_002c = 0;
    }

    if (self->field_001c) {
        Mem_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    ReleaseEmbeddedMembers(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr);

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    *(void***)self = CSF_VTBL(0x0082D410);
}

HOOK(0x00406090, C_0082d5e0_Destroy, "auto");
