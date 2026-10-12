#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Restores the C_008673b0 vtable, releases the pending handle on the owner
// (if any), unregisters this object from the owner, reverts to the base
// vtable, and frees the block when bit 0 of the argument is set.
static void** __fastcall C_008673b0_Destroy(C_008673b0* self, void* /*edx*/, char a2)
{
    uint32_t pendingHandle = self->field_0034;
    self->vtbl = CSF_VTBL(0x008673B0);

    if (pendingHandle) {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        // vtable slot 136 / 4 = 34
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, owner, 0)[136 / 4])(owner, nullptr, pendingHandle);
        self->field_0034 = 0;
    }

    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        // vtable slot 8 / 4 = 2
        ((void(__fastcall*)(void*, void*, C_008673b0*))CSF_AT(void**, owner, 0)[8 / 4])(owner, nullptr, self);
    }

    self->vtbl = CSF_VTBL(0x008670CC);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<void**>(self);
}

HOOK(0x738C40, C_008673b0_Destroy, "auto");
