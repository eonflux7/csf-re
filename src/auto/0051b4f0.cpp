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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00872abc_ReleaseBase, 0x0078C8E0, (void* self, void* edx));

static uint32_t* __fastcall C_00840dac_Destroy(C_00840dac* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x00840DAC);
    C_00872abc_ReleaseBase(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x51B4F0, C_00840dac_Destroy, "auto");
