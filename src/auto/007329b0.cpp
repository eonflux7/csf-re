#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_008670cc_ScalarDeletingDestructor(
    C_008670cc* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x008670CC);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x007329B0, C_008670cc_ScalarDeletingDestructor, "auto");
