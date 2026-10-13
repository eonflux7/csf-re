#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, GfxSkin_ReleaseBufferElements, 0x006B4DC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the buffer held in field +0x4 (after GfxSkin_ReleaseBufferElements
// with the companion value at +0x8), then frees the block and clears the three slots.
static int __fastcall ReleaseOwnedBufferSlots(int* self, void* /*edx*/) {
    int result = CSF_AT(int, self, 0x4);
    if (result) {
        GfxSkin_ReleaseBufferElements(CSF_AT(int, self, 0x4), CSF_AT(int, self, 0x8));
        result = CSF_AT(int, self, 0x4);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<intptr_t>(CSF_AT(int, self, 0x4))));
        }
    }
    CSF_AT(int, self, 0x4) = 0;
    CSF_AT(int, self, 0x8) = 0;
    CSF_AT(int, self, 0xC) = 0;
    return result;
}

HOOK(0x6B4E30, ReleaseOwnedBufferSlots, "auto");
