#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the child object held in field_0014 (calling its slot 0 with arg 1),
// clears the link, and frees this block if bit 0 of a2 is set.
static uint32_t* __fastcall C_0085e7d0_ReleaseChildAndMaybeFree(C_0085e7d0* self, void* /*edx*/, char a2) {
    void** childObject = reinterpret_cast<void**>(self->field_0014);
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x0085E7D0);
    if (childObject) {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, childObject, 0)[0])(childObject, nullptr, 1);
        self->field_0014 = 0;
    }
    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x69AF90, C_0085e7d0_ReleaseChildAndMaybeFree, "auto");
