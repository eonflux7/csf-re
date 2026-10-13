#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class release of owned entries for the C_00861d48 subobject chain.
CSF_FUNC(unsigned int, __fastcall, C_00861d48_ReleaseOwnedEntries, 0x006E6C70, (void* self, void* edx));

// Base-class teardown for C_00861d48.
CSF_FUNC(int, __fastcall, C_00861d48_Destroy, 0x006E70A0, (void* self, void* edx));

// Game heap free (Mem_Free = 0x691110).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destructor for C_008632c0: resets the vtable, releases the owned object (virtual slot 0
// with delete flag 1), clears the held pointer, runs the base-class teardown chain, and
// frees the block when bit 0 of the flag argument is set. Returns this.
static uint32_t* __fastcall C_008632c0_Destroy(C_008632c0* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x008632C0);

    void** ownedObject = reinterpret_cast<void**>(self->field_004c);
    if (ownedObject) {
        auto destroyOwned = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0]);
        destroyOwned(ownedObject, nullptr, 1);
        self->field_004c = 0;
    }

    C_00861d48_ReleaseOwnedEntries(self, nullptr);
    C_00861d48_Destroy(self, nullptr);

    if ((a2 & 1) != 0) {
        CoreMemory_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x706A00, C_008632c0_Destroy, "auto");
