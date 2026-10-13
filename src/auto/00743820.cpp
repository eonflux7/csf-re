#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(uint8_t, g_RtcErrorDispatchEnabled, 0x008A8926);
CSF_GLOBAL(void*, g_NetObjectWithRtcCounter, 0x008BC140);
CSF_GLOBAL(void*, g_NetCallbackPrimary, 0x008BC148);
CSF_GLOBAL(void*, g_NetCallbackSecondary, 0x008BC144);

CSF_FUNC(int, __cdecl, AiMountTasks_GetRtcNumErrors, 0x004A7DA0, ());
CSF_FUNC(bool, __fastcall, Net_TestRegisteredCallbackSlot, 0x007A9700, (void* self, void* edx));

static hook::Tail __fastcall NetCallbacks_DispatchRtcErrorCallback(void* self, const uint32_t* args)
{
    (void)self;
    (void)args;

    uint8_t al = g_RtcErrorDispatchEnabled;
    int eax = al;
    if (al == 0)
        return hook::tail_ret(eax);

    void* netObj = g_NetObjectWithRtcCounter;
    eax = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(netObj));
    if (netObj == nullptr)
        return hook::tail_ret(eax);

    int counterAtA8 = CSF_AT(int, netObj, 0xA8);
    int rtcNumErrors = AiMountTasks_GetRtcNumErrors();
    eax = rtcNumErrors;
    if (counterAtA8 >= rtcNumErrors)
        return hook::tail_ret(eax);

    void* netA = g_NetCallbackPrimary;
    if (netA == nullptr)
        return hook::tail_ret(eax);

    int busyA = ((int(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(netA))[0x0C / 4])(netA, nullptr);
    eax = static_cast<uint8_t>(busyA);
    if (eax != 0)
        return hook::tail_ret(eax);

    void* netB = g_NetCallbackSecondary;
    if (netB == nullptr)
        return hook::tail_ret(eax);

    bool slotReady = Net_TestRegisteredCallbackSlot(netB, nullptr);
    eax = static_cast<uint8_t>(slotReady);
    if (!slotReady)
        return hook::tail_ret(eax);

    int busyB = ((int(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(netB))[0x0C / 4])(netB, nullptr);
    eax = static_cast<uint8_t>(busyB);
    if (eax != 0)
        return hook::tail_ret(eax);

    netA = g_NetCallbackPrimary;
    int notifyA = ((int(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(netA))[0x04 / 4])(netA, nullptr);
    eax = static_cast<uint8_t>(notifyA);
    if (eax == 0)
        return hook::tail_ret(eax);

    netB = g_NetCallbackSecondary;
    return hook::tail_jump(netB, (*reinterpret_cast<void***>(netB))[0x04 / 4]);
}

HOOK_TAIL(0x00743820, NetCallbacks_DispatchRtcErrorCallback, "net_callbacks");
