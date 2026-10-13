#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the buffer owned by C_008453a4 and consumes its flag (returns char, ignored here).
CSF_FUNC(char, __fastcall, C_008453a4_ReleaseOwnedBufferAndConsumeFlag, 0x0055BE20, (C_008453a4* self, void* edx));

// Resets the C_00845368 base part of the object and clears its flag (returns char, ignored here).
CSF_FUNC(char, __fastcall, C_00845368_ResetVtableAndClearFlag, 0x0055BD40, (uint8_t* self, void* edx));

// Resets the embedded member located at this+0x08 (returns void*, ignored here).
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));

// Game heap free (cdecl, caller cleans the stack).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-like teardown: installs the class vtable, releases owned resources,
// resets the embedded member at +0x08 and the base part, and frees the block when
// bit 0 of the argument is set. Returns this.
static uint8_t* __fastcall C_008453a4_DestroyAndFree(C_008453a4* self, void* /*edx*/, char freeFlags)
{
    *reinterpret_cast<uint32_t*>(self) = reinterpret_cast<uint32_t>(CSF_VTBL(0x008453A4));

    C_008453a4_ReleaseOwnedBufferAndConsumeFlag(self, nullptr);

    ResetEmbeddedMemberAt8(
        reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + 8), nullptr);

    C_00845368_ResetVtableAndClearFlag(reinterpret_cast<uint8_t*>(self), nullptr);

    if ((freeFlags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<uint8_t*>(self);
}

HOOK(0x0055BDA0, C_008453a4_DestroyAndFree, "auto");
