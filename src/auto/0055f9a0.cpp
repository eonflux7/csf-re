#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, MenuMpOnline_TestEventEligibility, 0x007A8ED0,
         (int event));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static int __fastcall MenuMpOnline_HandleMatchTransitionEvent(
    void* self, void* /*edx*/, int event, int /*unused*/)
{
    int result = MenuMpOnline_TestEventEligibility(event);
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    void* menu = static_cast<std::uint8_t*>(self) - 4;
    int online;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        online = g_OnlineMatchFlag;
    }
    else
    {
        online = g_MpNetworkType == 3;
    }

    if (online)
    {
        return reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, menu, 0)[0x4C / 4])(menu, nullptr);
    }

    void** vtable = CSF_AT(void**, menu, 0);
    CSF_AT(int, menu, 0x18) = 2;
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        vtable[0x4C / 4])(menu, nullptr);
}

HOOK(0x0055F9A0, MenuMpOnline_HandleMatchTransitionEvent, "auto");
