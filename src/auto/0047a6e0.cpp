#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, sub_476220, 0x00476220, (void* self, void* edx, int a2));

static int __fastcall C_008316d8_GetResolvedSubObjectValue(
    C_008316d8* self, void* /*edx*/)
{
    sub_476220(self, nullptr, 0);
    int result = self->vResolveEntryLookupValue();
    if (result)
        return C_008319c0_GetSubObjectField74(
            reinterpret_cast<C_008319c0*>(self), nullptr);
    return result;
}

HOOK(0x0047A6E0, C_008316d8_GetResolvedSubObjectValue, "auto");
