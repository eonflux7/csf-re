#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_0083b668_Destroy(C_0083b668* self, void* /*edx*/, char freeSelfFlag)
{
    CSF_AT(void*, self, 0x40) = CSF_VTBL(0x0082D410);
    C_0083b958_Destroy(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4D03C0, C_0083b668_Destroy, "auto");
