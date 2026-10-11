#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Embedded member destructor helper at 0x6AEEC0 (__fastcall, edx unused, ret 0).
CSF_FUNC(int, __fastcall, ReleaseEmbeddedMember, 0x006AEEC0, (void* self, void* edx));

// Game heap free (__cdecl, ret 0).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys a C_0082e684 object: resets its vtable to C_0082e684, releases the
// embedded member at +0x4, restores the base C_0082d410 vtable, and frees the
// block when bit 0 of the flags argument is set. Returns the object pointer.
static uint32_t* __fastcall C_0082e684_Destroy(C_0082e684* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082E684);
    ReleaseEmbeddedMember(reinterpret_cast<void*>(&self->field_0004), nullptr);
    self->vtbl = CSF_VTBL(0x0082D410);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x41EA00, C_0082e684_Destroy, "auto");
