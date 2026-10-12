#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body of C_0082dc10: releases the owned sub-object at +0x10 (virtual slot 0 with arg 1),
// runs the base cleanup via C_0082deb8_ReleaseChildBase, restores the C_0082d410 vtable, and frees
// the block when bit 0 of a2 is set.
static void* __fastcall C_0082dc10_Destroy(C_0082dc10* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082DC10);

    void* owned = reinterpret_cast<void*>(self->field_0010);
    if (owned) {
        // Virtual slot 0 of the owned object, called with arg 1.
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, owned, 0)[0])(owned, nullptr, 1);
        self->field_0010 = 0;
    }

    C_0082deb8_ReleaseChildBase_41E110(reinterpret_cast<C_0082e520*>(self), nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<void*>(self);
}

HOOK(0x0040D0F0, C_0082dc10_Destroy, "auto");
