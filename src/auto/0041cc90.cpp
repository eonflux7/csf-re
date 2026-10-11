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
#include "legacy/string.h"

// Callee bound by address: base-class release (j_j_C_0082deb8_ReleaseChildBase -> 0x418430).
CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase, 0x00418430, (void* self, void* edx));

// Callee bound by address: unnamed cleanup step on C_0082e1e8 subobject state.
CSF_FUNC(int, __fastcall, ClearSubobjectState_418AE0, 0x00418AE0, (void* self, void* edx));

// Game heap free (Mem_Free = 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown for C_0082e3b8: releases the owned child (virtual
// destructor with delete flag 1), clears the owned pointer, runs base cleanup,
// and frees the block when bit 0 of the flag argument is set. Returns this.
static uint32_t* __fastcall C_0082e3b8_Destroy(C_0082e3b8* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x0082E3B8);

    void** ownedChild = reinterpret_cast<void**>(self->field_0030);
    if (ownedChild) {
        auto destroyOwned = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedChild, 0)[0]);
        destroyOwned(ownedChild, nullptr, 1);
        self->field_0030 = 0;
    }

    C_0082deb8_ReleaseChildBase(self, nullptr);
    ClearSubobjectState_418AE0(self, nullptr);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x41CC90, C_0082e3b8_Destroy, "auto");
