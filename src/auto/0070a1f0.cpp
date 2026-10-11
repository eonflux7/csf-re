#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resets the state of the object at this+0x00 (returns char, ignored here).
CSF_FUNC(char, __fastcall, sub_70CC90, 0x0070CC90, (void* self, void* edx));

// Resets the embedded member located at this+0xDC (returns int, ignored here).
CSF_FUNC(int, __fastcall, sub_709AE0, 0x00709AE0, (void* self, void* edx));

// Resets the object's remaining state (returns char, ignored here).
CSF_FUNC(char, __fastcall, sub_6FE010, 0x006FE010, (void* self, void* edx));

// Game heap free (cdecl, caller cleans the stack).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Installs the class vtable, resets the embedded member at +0xDC, resets the
// remaining state, and frees the block when bit 0 of the argument is set.
// Returns this.
static int* __fastcall S_006fdfe0_DestroyAndFree(S_006fdfe0* self, void* /*edx*/, char freeFlags)
{
    self->field_0000 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0086350C));

    sub_70CC90(self, nullptr);

    sub_709AE0(reinterpret_cast<uint8_t*>(self) + 0xDC, nullptr);

    sub_6FE010(self, nullptr);

    if ((freeFlags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<int*>(self);
}

HOOK(0x0070A1F0, S_006fdfe0_DestroyAndFree, "auto");
