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

CSF_FUNC(void, __fastcall, C_00840ea4_Construct, 0x0051C140, (C_00840ea4* self, void*));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00840f00_Destroy(C_00840f00* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00840F00);
    C_00840ea4_Construct(reinterpret_cast<C_00840ea4*>(self), nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x51DB40, C_00840f00_Destroy, "auto");
