#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees of CMenuMpSessions_ShowFriendlyFireOptionState
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x006A6D70, ());
CSF_FUNC(std::uint32_t, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ForwardToSub6687B0AtOffset9c, 0x0073D4C0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CMenuMpSessions_RefreshOptionState, 0x00562770, (CMenuMpSessions* self, void* edx, char a3, char a4));

// Data referenced by the original
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static int __fastcall CMenuMpSessions_ShowFriendlyFireOptionState(CMenuMpSessions* self, void* /*edx*/, int /*a3*/, int /*a4*/)
{
    // Disable/reset the first panel object (field_0070), vtable slot 26 (+0x68), args (0, 1, 1)
    void* firstPanelObj = reinterpret_cast<void*>(self->field_0070);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, firstPanelObj, 0)[26])(firstPanelObj, nullptr, 0, 1, 1);

    // Same reset on the second object (field_0034), vtable slot 26 (+0x68), args (0, 1, 1)
    void* secondPanelObj = reinterpret_cast<void*>(self->field_0034);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, secondPanelObj, 0)[26])(secondPanelObj, nullptr, 0, 1, 1);

    // Fetch the control "stFuegoAmigo" from the object at field_0040, vtable slot 63 (+0xFC)
    void* lookupObj = reinterpret_cast<void*>(self->field_0040);
    self->field_01e8 = 0;
    using GetControlByNameFn = std::uint32_t(__fastcall*)(void*, void*, const char*);
    self->field_006c = reinterpret_cast<GetControlByNameFn>(
        CSF_AT(void**, lookupObj, 0)[63])(lookupObj, nullptr, CSF_STR(0x00845A3C, "stFuegoAmigo"));

    // Determine whether this is an online match
    bool isOnlineMatch;
    if (GetGlobal8B0688() != 0)
    {
        GetGlobal8B0688();
        isOnlineMatch = g_OnlineMatchFlag != 0;
    }
    else
    {
        isOnlineMatch = g_MpNetworkType == 3;
    }

    if (isOnlineMatch)
    {
        // Global state object, vtable slot 51 (+0xCC), no args
        void* globalState = GetGlobalState();
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, globalState, 0)[51])(globalState, nullptr);
    }
    else
    {
        std::uint32_t forwardTargetAddr = GetGlobalDword_73C020();
        ForwardToSub6687B0AtOffset9c(reinterpret_cast<void*>(forwardTargetAddr), nullptr);
    }

    const bool refreshResult = CMenuMpSessions_RefreshOptionState(self, nullptr, 1, 0);
    self->field_0014 = 7;
    // Original returns the callee's al (0/1) zero-extended into eax, so return as a 32-bit int.
    return refreshResult ? 1 : 0;
}

HOOK(0x00563350, CMenuMpSessions_ShowFriendlyFireOptionState, "auto");
