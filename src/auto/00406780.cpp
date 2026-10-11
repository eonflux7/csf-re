#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original callee: resets the base-class (C_0082d410) state of the object.
CSF_FUNC(void, __fastcall, C_0082d410_ReleaseBaseResources, 0x004059B0, (void* self, void* edx));

// Game heap free, used by the deleting variant.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys a C_0082d70c object: restores its vtables, releases the object held at +0x40
// through its virtual slot 11 (deleting call with flag 1), resets the base state, and
// frees the block when bit 0 of the flag argument is set. Returns the object address.
static uint32_t* __fastcall C_0082d70c_Destroy(C_0082d70c* self, void* /*edx*/, char deleteFlag)
{
    uint32_t ownedObject = CSF_AT(uint32_t, self, 0x40);

    self->vtbl = CSF_VTBL(0x0082D70C);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D6F0));

    if (ownedObject != 0)
    {
        void* owned = reinterpret_cast<void*>(ownedObject);
        // Slot 0x2C / 4 = 11: virtual destroy with flag 1.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, owned, 0)[11])(owned, nullptr, 1);
        CSF_AT(uint32_t, self, 0x40) = 0;
    }

    C_0082d410_ReleaseBaseResources(self, nullptr);

    if ((deleteFlag & 1) != 0)
    {
        Mem_Free(self);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x406780, C_0082d70c_Destroy, "auto");
