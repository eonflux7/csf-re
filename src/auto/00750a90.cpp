#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, NetChatDDL_RegisterAppName, 0x0067AA80, (void* a1));
CSF_FUNC(int, __cdecl, NetChatDDL_SetNetworkKey, 0x00681C60, (char* a1, int a2));
CSF_FUNC(char, __fastcall, NetPostman_Reset, 0x0067D040, (void* self, void* edx));
CSF_FUNC(char, __cdecl, NetChatDDL_StepSetupA, 0x007AABC0, ());
CSF_FUNC(char, __cdecl, NetChatDDL_StepSetupB, 0x007A4B50, ());
CSF_FUNC(char, __cdecl, NetChatDDL_StepSetupC, 0x007A0200, ());
CSF_FUNC(char, __cdecl, NetChatDDL_StepSetupD, 0x007A4B00, ());

static char __cdecl NetChatDDL_InitNetworkSession()
{
    NetChatDDL_RegisterAppName(
        const_cast<char*>(CSF_STR(0x00868910, "Commandos3D")));

    // The previous cdecl argument remains on the stack as argument two.
    NetChatDDL_SetNetworkKey(
        const_cast<char*>(CSF_STR(0x008688F4, "2F5AB8CD3122EB5519F892BC")),
        static_cast<int>(reinterpret_cast<std::intptr_t>(
            CSF_STR(0x00868910, "Commandos3D"))));

    NetPostman_Reset(
        reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC1B8)), nullptr);

    NetChatDDL_StepSetupA();
    NetChatDDL_StepSetupB();
    NetChatDDL_StepSetupC();
    NetChatDDL_StepSetupD();

    // Inline the final forwarding stub's effect.
    return NetPostman_Reset(
        reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BC280)), nullptr);
}

HOOK(0x750A90, NetChatDDL_InitNetworkSession, "auto");
