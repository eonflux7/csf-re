#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class release thunk at 0x40D170 (j_C_0082deb8_ReleaseChildBase).
CSF_FUNC(int, __fastcall, ReleaseChildBase_40D170, 0x0040D170, (void* self, void* edx));

// Unnamed cleanup step at 0x40D360, run after the child release.
CSF_FUNC(int, __fastcall, ClearSubobjectState_40D360, 0x0040D360, (void* self, void* edx));

// Game heap free (Mem_Free = 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown for C_0082dcf0: resets the vtable, releases the owned
// child (virtual destructor slot 0 with delete flag 1), runs the base release and
// cleanup steps, and frees the block when bit 0 of the flag argument is set.
// Returns this.
static uint32_t* __fastcall C_0082dcf0_Destroy(C_0082dcf0* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082DCF0);

    void** ownedChild = reinterpret_cast<void**>(self->field_0030);
    if (ownedChild) {
        auto destroyOwned = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedChild, 0)[0]);
        destroyOwned(ownedChild, nullptr, 1);
        self->field_0030 = 0;
    }

    ReleaseChildBase_40D170(self, nullptr);
    ClearSubobjectState_40D360(self, nullptr);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x40DDC0, C_0082dcf0_Destroy, "auto");
