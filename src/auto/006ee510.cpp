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

CSF_FUNC(void, __fastcall, C_008605d8_Destroy_6BE050, 0x006BE050, (C_008605d8* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00862084_Destroy(C_00862084* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00862084);
    C_008605d8_Destroy_6BE050(reinterpret_cast<C_008605d8*>(self), nullptr);
    if ((freeSelfFlag & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6EE510, C_00862084_Destroy, "auto");
