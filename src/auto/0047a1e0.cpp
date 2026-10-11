#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __fastcall C_0085e7b8_CanSelectItem(
    C_0085e7b8* self, void* /*edx*/, C_008319c0* item)
{
    if (!item)
        return 1;

    const int type = C_008319c0_GetSubObjectField74(
        reinterpret_cast<C_008319c0*>(self), nullptr);

    if (type == 0 || type == 4)
    {
        void* itemProvider = GetCachedObjectProvider(item, nullptr);
        return GetCachedObjectProvider(self, nullptr) == itemProvider ? 1 : 0;
    }

    if (type == 21)
        return C_008319c0_GetSubObjectField74(item, nullptr) == 1 ? 1 : 0;

    return 1;
}

HOOK(0x0047A1E0, C_0085e7b8_CanSelectItem, "auto");
