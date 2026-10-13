#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReleaseSceneLightSubobject, 0x004F0810, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown for C_0083ef04: releases the owned sub-object
// (if any) with flag 3, clears the pointer, restores the base vtable, and
// frees the object itself when bit 0 of a2 is set.
static int* __fastcall C_0083ef04_Destroy(C_0083ef04* self, void* /*edx*/, char a2) {
    self->vtbl = CSF_VTBL(0x0083EF04);

    uint32_t ownedSubobject = self->field_0004;
    if (ownedSubobject) {
        ReleaseSceneLightSubobject(reinterpret_cast<void*>(ownedSubobject), nullptr, 3);
        self->field_0004 = 0;
    }

    self->vtbl = CSF_VTBL(0x0083EEFC);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<int*>(self);
}

HOOK(0x4F09B0, C_0083ef04_Destroy, "auto");
