#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class release (C_0082deb8_ReleaseChildBase = 0x41E110).
CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));

// Unnamed cleanup step run after the base release (0x40F760).
CSF_FUNC(int, __fastcall, ClearStateAfterRelease_40F760, 0x0040F760, (void* self, void* edx));

// Game heap free (Mem_Free = 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown for C_0082dd78: releases the owned child at +0x30
// (virtual slot 0 with delete flag 1), runs the base release and cleanup steps,
// and frees the block when bit 0 of the flag argument is set. Returns this.
static uint32_t* __fastcall C_0082dd78_Destroy(C_0082dd78* self, void* /*edx*/, char a2)
{
    void** ownedChild = reinterpret_cast<void**>(self->field_0030);
    self->vtbl = CSF_VTBL(0x0082DD78);

    if (ownedChild) {
        auto destroyOwned = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedChild, 0)[0]);
        destroyOwned(ownedChild, nullptr, 1);
        self->field_0030 = 0;
    }

    C_0082deb8_ReleaseChildBase_41E110(self, nullptr);
    ClearStateAfterRelease_40F760(self, nullptr);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x40E780, C_0082dd78_Destroy, "auto");
