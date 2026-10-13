#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_00840d54_BaseRelease, 0x00519810, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Cleanup for C_00840d54: stamps this object's vtable, runs the base-class
// release, then frees the owned block (if any) through the game heap.
static int __fastcall C_00840d54_Cleanup(C_00840d54* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840D54);
    C_00840d54_BaseRelease(self, nullptr);

    uint32_t ownedBlockAddr = self->field_0008;
    int result = static_cast<int>(ownedBlockAddr);
    if (result) {
        result = CoreMemory_Free(reinterpret_cast<void*>(ownedBlockAddr));
        self->field_0008 = 0;
    }
    return result;
}

HOOK(0x00519850, C_00840d54_Cleanup, "auto");
