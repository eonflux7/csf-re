#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (as given in the packet).
CSF_FUNC(int, __cdecl, sub_44BFC0, 0x0044BFC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Clears the additional-resources slot of a SceneLoader: releases the stored
// resource through sub_44BFC0 and frees the block at +0x10 with the game's
// debug allocator, then resets the bookkeeping fields. Returns the freed
// block's result (or 0 when nothing was held).
static int __fastcall SceneLoader_ClearAdditionalResources(int* self, void* /*edx*/) {
    int result = CSF_AT(int, self, 0x10);
    if (result) {
        sub_44BFC0(result, CSF_AT(int, self, 0x14));
        result = CSF_AT(int, self, 0x10);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
        }
    }
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(int, self, 0x1C) = -1;
    return result;
}

HOOK(0x0044C540, SceneLoader_ClearAdditionalResources, "auto");
