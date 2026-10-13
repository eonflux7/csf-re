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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

static uint32_t* __fastcall C_00862ef8_Destroy(C_00862ef8* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00862EF8);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
    if ((freeSelfFlag & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6FCCA0, C_00862ef8_Destroy, "auto");
