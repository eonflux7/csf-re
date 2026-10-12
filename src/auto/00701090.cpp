#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(unsigned int, __fastcall, ApplySceneEntryValue, 0x006B1840, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destroys a C_0086315c: resets its vtable, applies the pending scene entry value
// (if any) through the global owner, runs the base-class teardown, and frees the
// object when bit 0 of the flag argument is set.
static int* __fastcall C_0086315c_Destroy(C_0086315c* self, void* /*edx*/, char freeAfterDestroy)
{
    uint32_t sceneEntryValue = self->field_0028;
    self->vtbl = CSF_VTBL(0x0086315C);

    if (sceneEntryValue != 0)
    {
        void* sceneOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
        ApplySceneEntryValue(sceneOwner, nullptr, static_cast<int>(sceneEntryValue));
        self->field_0028 = 0;
    }

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeAfterDestroy & 1) != 0)
    {
        Mem_Free(self);
    }

    return reinterpret_cast<int*>(self);
}

HOOK(0x00701090, C_0086315c_Destroy, "auto");
