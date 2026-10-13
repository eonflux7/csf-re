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

CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00860a30_ReleaseParentState, 0x006C9F40, (C_00860a30* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// __thiscall C_008622e8::ReleaseAndOptionallyFree(char freeStorage), ret 4
static C_008622e8* __fastcall C_008622e8_ReleaseAndOptionallyFree(C_008622e8* self, void* /*edx*/, char freeStorage)
{
    self->vtbl = CSF_VTBL(0x008622E8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008622E0));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x008622C4));

    RpPrtStd_ResetBaseState(self, nullptr);
    C_00860a30_ReleaseParentState(reinterpret_cast<C_00860a30*>(self), nullptr);

    if ((freeStorage & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x6F0710, C_008622e8_ReleaseAndOptionallyFree, "auto");
