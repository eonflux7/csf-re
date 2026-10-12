#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class release for the C_0082deb8 subobject chain.
CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));

// Base-class state teardown for C_0082e1e8.
CSF_FUNC(int, __fastcall, C_0082e1e8_Destroy_418AE0, 0x00418AE0, (C_0082e1e8* self, void*));

// Game heap free (Mem_Free = 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor for C_0082db60: releases the owned child object (virtual slot 0 with
// delete flag 1), clears the held pointer, runs the base-class teardown chain,
// and frees the block when bit 0 of the flag argument is set. Returns this.
static uint32_t* __fastcall C_0082db60_Destroy(C_0082db60* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082DB60);

    void** ownedObject = reinterpret_cast<void**>(self->field_008c);
    if (ownedObject) {
        auto destroyOwned = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0]);
        destroyOwned(ownedObject, nullptr, 1);
        self->field_008c = 0;
    }

    C_0082deb8_ReleaseChildBase_41E110(self, nullptr);
    C_0082e1e8_Destroy_418AE0(self, nullptr);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x40CE60, C_0082db60_Destroy, "auto");
