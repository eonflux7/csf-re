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

CSF_FUNC(int, __fastcall, C_008769d4_ReleaseBase, 0x00403E10, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_008769ec_Destroy(C_008769ec* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x008769EC);
    C_008769d4_ReleaseBase(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x43EF90, C_008769ec_Destroy, "auto");
