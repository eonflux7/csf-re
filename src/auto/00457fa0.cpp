#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Sets the three vtable slots to C_0082d410's vtable and, when bit 0 of the
// flag argument is set, releases the block through the game heap.
static void* __fastcall C_0082d410_ResetVtableAndMaybeFree(void* self, void* /*edx*/, char releaseFlag) {
    void* vtable = CSF_VTBL(0x0082D410);
    CSF_AT(void*, self, 0x10) = vtable;
    CSF_AT(void*, self, 0x04) = vtable;
    CSF_AT(void*, self, 0x00) = vtable;

    if ((releaseFlag & 1) != 0) {
        Mem_Free(self);
    }
    return self;
}

HOOK(0x457FA0, C_0082d410_ResetVtableAndMaybeFree, "auto");
