#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CoreZlibLog_EndDeflate, 0x006A07F0, (void* stream));
CSF_FUNC(int, __cdecl, ZlibLog_ReleaseStreamHandle, 0x006A18C0, (uint32_t* a1));

// Destructor body for C_0085e748: releases the zlib stream, its buffer and the object block.
static int __fastcall C_0085e748_Destroy(C_0085e748* self, void* /*edx*/) {
    uint8_t usesReleaseHandle = self->field_0008;
    self->vtbl = CSF_VTBL(0x0085E748);

    if (usesReleaseHandle) {
        ZlibLog_ReleaseStreamHandle(reinterpret_cast<uint32_t*>(self->field_0010));
    } else {
        CoreZlibLog_EndDeflate(reinterpret_cast<void*>(self->field_0010));
    }

    if (self->field_000c) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    int result = static_cast<int>(self->field_0010);
    if (result) {
        result = CoreMemory_Free(reinterpret_cast<void*>(self->field_0010));
        self->field_0010 = 0;
    }

    self->vtbl = CSF_VTBL(0x0085E738);
    return result;
}

HOOK(0x00699DE0, C_0085e748_Destroy, "auto");
