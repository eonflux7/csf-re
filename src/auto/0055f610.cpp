#include <cstdint>
#include "hook.h"
#include "globals.h"

// Callees bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, Game_DispatchMenuRequest, 0x0050F790,
         (void* self, void* edx, int dialogId, int a3, int a4, int a5));

// Hooked function: thiscall, one stack arg (ret 4).
static unsigned int __fastcall MenuMpOnline_HandleConnectionState(void* self, void* /*edx*/, int /*a2*/)
{
    void* stateManager = GetGlobalState_6A6D70();
    int stateIsSet = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateManager, 0)[7])(stateManager, nullptr);

    if (stateIsSet != 0)
    {
        void* stateManagerAgain = GetGlobalState_6A6D70();
        unsigned int result = ((unsigned int(__fastcall*)(void*, void*))CSF_AT(void**, stateManagerAgain, 0)[7])(stateManagerAgain, nullptr);
        if (result == 2)
        {
            void* globalState = GetGlobalState_50D190();
            result = static_cast<unsigned int>(GlobalState_ResetPending(globalState, nullptr));
            CSF_AT(uint32_t, self, 0x18) = 4;
        }
        return result;
    }

    void* globalStateForReset = GetGlobalState_50D190();
    GlobalState_ResetPending(globalStateForReset, nullptr);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[19])(self, nullptr);
    void* globalStateForDialog = GetGlobalState_50D190();
    return Game_DispatchMenuRequest(globalStateForDialog, nullptr, 0x4C413331, 1, 1, 0);
}

HOOK(0x0055F610, MenuMpOnline_HandleConnectionState, "auto");
