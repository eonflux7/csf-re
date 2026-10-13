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

CSF_FUNC(void, __fastcall, C_008634c0_SetVtable, 0x00708860, (C_008634c0* self, void* /*edx*/));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00862944_Destroy(C_00862944* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00862944);
    C_008634c0_SetVtable(reinterpret_cast<C_008634c0*>(self), nullptr);
    if ((freeSelfFlag & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6F7F50, C_00862944_Destroy, "auto");
