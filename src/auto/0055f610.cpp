#include <cstdint>
#include "hook.h"
#include "globals.h"

// Callees bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetActiveStateManager, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, MenuMpOnline_ResetOnlineSession, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MenuMpOnline_ShowDialogById, 0x0050F790,
         (void* self, void* edx, int dialogId, int a3, int a4, int a5));

// Hooked function: thiscall, one stack arg (ret 4).
static unsigned int __fastcall MenuMpOnline_HandleConnectionState(void* self, void* /*edx*/, int /*a2*/)
{
    void* stateManager = GetActiveStateManager();
    int stateIsSet = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateManager, 0)[7])(stateManager, nullptr);

    if (stateIsSet != 0)
    {
        void* stateManagerAgain = GetActiveStateManager();
        unsigned int result = ((unsigned int(__fastcall*)(void*, void*))CSF_AT(void**, stateManagerAgain, 0)[7])(stateManagerAgain, nullptr);
        if (result == 2)
        {
            void* globalState = GetGlobalState();
            result = static_cast<unsigned int>(MenuMpOnline_ResetOnlineSession(globalState, nullptr));
            CSF_AT(uint32_t, self, 0x18) = 4;
        }
        return result;
    }

    void* globalStateForReset = GetGlobalState();
    MenuMpOnline_ResetOnlineSession(globalStateForReset, nullptr);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[19])(self, nullptr);
    void* globalStateForDialog = GetGlobalState();
    return MenuMpOnline_ShowDialogById(globalStateForDialog, nullptr, 0x4C413331, 1, 1, 0);
}

HOOK(0x0055F610, MenuMpOnline_HandleConnectionState, "auto");
