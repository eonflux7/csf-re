#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets the object's vtable to C_00867768, clears the dword at +8 of the
// block referenced by field_0004 (if any), and frees the object itself when
// bit 0 of the flags argument is set. Returns the object's vtable slot address
// (which equals `this`).
static uint32_t* __fastcall C_00867768_DestructWithOptionalFree(C_00867768* self, void* /*edx*/, char a2) {
    uint32_t linkedBlockAddr = self->field_0004;
    self->vtbl = CSF_VTBL(0x00867768);
    if (linkedBlockAddr) {
        uint8_t* linkedBlock = reinterpret_cast<uint8_t*>(linkedBlockAddr);
        CSF_AT(uint32_t, linkedBlock, 0x8) = 0;
    }
    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<uint32_t*>(&self->vtbl);
}

HOOK(0x0073F770, C_00867768_DestructWithOptionalFree, "auto");
