#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, NetSpawnMessages_CreateType50Message, 0x00751DE0, ());
CSF_FUNC(char, __fastcall, Net_HandleCreatedMessage, 0x007AAB40, (void* self, void* edx, char a2, int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, AiMountTasks_GetRtcNumErrors, 0x004A7DA0, ());
CSF_FUNC(uint32_t*, __fastcall, NetSpawnMessages_InitializeTimedEntry, 0x00751B20, (uint32_t* self, void* edx, int value));

CSF_GLOBAL(uint32_t*, g_Type50Message, 0x008BC13C);
CSF_GLOBAL(uint32_t, g_NetCallbackField12C, 0x008BC12C);
CSF_GLOBAL(uint32_t, g_NetCallbackField130, 0x008BC130);
CSF_GLOBAL(uint32_t, g_NetCallbackField134, 0x008BC134);
CSF_GLOBAL(uint32_t, g_NetCallbackField138, 0x008BC138);
CSF_GLOBAL(uint32_t*, g_TimedEntry, 0x008BC150);
CSF_GLOBAL(uint32_t, g_NetCallbackField154, 0x008BC154);
CSF_GLOBAL(uint32_t, g_NetCallbackField158, 0x008BC158);

static char __cdecl NetCallbacks_InitializeState()
{
    uint32_t* type50Message = NetSpawnMessages_CreateType50Message();
    g_Type50Message = type50Message;
    if (type50Message)
    {
        Net_HandleCreatedMessage(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC294)), nullptr, 0, reinterpret_cast<int>(type50Message));
    }

    g_NetCallbackField12C = 0;
    g_NetCallbackField130 = 0;
    g_NetCallbackField134 = 0;
    g_NetCallbackField138 = 0;

    uint32_t* timedEntryStorage = static_cast<uint32_t*>(Mem_Alloc(12));
    if (timedEntryStorage)
    {
        int rtcNumErrors = AiMountTasks_GetRtcNumErrors();
        g_TimedEntry = NetSpawnMessages_InitializeTimedEntry(timedEntryStorage, nullptr, rtcNumErrors);
    }
    else
    {
        g_TimedEntry = nullptr;
    }

    g_NetCallbackField154 = 0;
    g_NetCallbackField158 = 0;
    return 1;
}

HOOK(0x743980, NetCallbacks_InitializeState, "auto");
