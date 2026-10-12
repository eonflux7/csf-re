#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall C_0083f1d8_Destroy(
    C_0083f1d8* self, void* /*edx*/, char flags)
{
    self->vtbl = CSF_VTBL(0x0083F1D8);
    Hud_UnregisterObject(reinterpret_cast<int>(self));
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x004F9270, C_0083f1d8_Destroy, "auto");
