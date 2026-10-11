#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpLtMapGetRenderStyle, 0x0056EFF0, ());
CSF_FUNC(int, __cdecl, RpLtMapSetRenderStyle, 0x0056F2B0, (int style, int context));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall C_008605d8_SetEnvironmentFlag(
    C_008605d8* self, void* /*edx*/, int flag, int enabled)
{
    if (static_cast<std::uint8_t>(enabled))
        self->field_008c |= flag;
    else
        self->field_008c &= ~flag;

    switch (flag)
    {
    case 8:
    {
        int style = RpLtMapGetRenderStyle();
        if (static_cast<std::uint8_t>(enabled))
            style |= 1;
        else
            style &= static_cast<int>(0xFFFFFFFE);
        return RpLtMapSetRenderStyle(style, static_cast<int>(self->field_0004));
    }
    case 16:
    {
        int style = RpLtMapGetRenderStyle();
        if (static_cast<std::uint8_t>(enabled))
            style |= 2;
        else
            style &= static_cast<int>(0xFFFFFFFD);
        return RpLtMapSetRenderStyle(style, static_cast<int>(self->field_0004));
    }
    case 32:
    {
        int managerId = GetGlobalInt();
        void* manager = reinterpret_cast<void*>(
            ResolveClumpManager(managerId, nullptr));
        return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, manager, 0)[0x64 / 4])(
                manager, nullptr, enabled);
    }
    default:
        return flag;
    }
}

HOOK(0x006BC830, C_008605d8_SetEnvironmentFlag, "auto");
