#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));

// Destroys a C_0085ea2c object: closes its scene file if it owns one, restores the
// base-class vtable, and frees the block when the low bit of the flag is set.
static void* __fastcall C_0085ea2c_Destroy(C_0085ea2c* self, void* /*edx*/, char freeFlag) {
    const std::uint8_t ownsSceneFile = self->field_0008;

    *(void***)self = CSF_VTBL(0x0085EA2C);
    if (ownsSceneFile && self->field_0004) {
        CloseSceneFile(reinterpret_cast<void*>(self->field_0004));
    }

    *(void***)self = CSF_VTBL(0x0085E738);
    if ((freeFlag & 1) != 0) {
        CoreMemory_Free(reinterpret_cast<void*>(self));
    }

    return reinterpret_cast<void*>(self);
}

HOOK(0x0069D6C0, C_0085ea2c_Destroy, "auto");
