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

CSF_FUNC(int, __fastcall, C_0085e7d8_Destroy_69BE60, 0x0069BE60, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall CTextDatabase_Destroy(void* self, void* /*edx*/, char freeSelfFlag)
{
    *(void***)self = CSF_VTBL(0x0083EE68);
    C_0085e7d8_Destroy_69BE60(self, nullptr);
    if ((freeSelfFlag & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4EF050, CTextDatabase_Destroy, "auto");
