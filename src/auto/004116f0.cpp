#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase, 0x0041E110, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body of C_0082df70: releases the owned sub-object at +0x10 (virtual slot 0 with arg 1),
// runs the base cleanup, restores the C_0082d410 vtable, and frees the block if bit 0 of a2 is set.
static void* __fastcall C_0082df70_Destroy(C_0082df70* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082DF70);

    void* owned = reinterpret_cast<void*>(self->field_0010);
    if (owned) {
        // Virtual slot 0 of the owned object, called with arg 1.
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, owned, 0)[0])(owned, nullptr, 1);
        self->field_0010 = 0;
    }

    C_0082deb8_ReleaseChildBase(self, nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<void*>(self);
}

HOOK(0x004116F0, C_0082df70_Destroy, "auto");
