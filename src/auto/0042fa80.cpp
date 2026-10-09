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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void, __fastcall, DestroyEmbeddedMember8, 0x006C4A50, (void* self, void* edx));

static C_008305f8* __fastcall C_008305f8_Destroy(C_008305f8* self, void* /*edx*/, char freeFlag)
{
    *(void***)self = CSF_VTBL(0x00830918);
    DestroyEmbeddedMember8(reinterpret_cast<uint8_t*>(self) + 8, nullptr);

    *(void***)self = CSF_VTBL(0x008305F8);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeFlag & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x0042FA80, C_008305f8_Destroy, "auto");
