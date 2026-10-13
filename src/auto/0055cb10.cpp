#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __fastcall, MenuAccounts_FallbackToList, 0x0055C9D0, (char* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));

static int __fastcall S_0055c780_ProcessAccountSelection(
    S_0055c780* self, void* /*edx*/, int /*unused*/)
{
    void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    int result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, state, 0)[0x1C / 4])(state, nullptr);

    if (result != 0)
    {
        if (result == 6)
        {
            C_00861120* globalState =
                reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
            result = reinterpret_cast<int>(
                C_00861120_GetSchedulerProxy(globalState, nullptr));
            if (result != 0)
            {
                state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
                result = ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, state, 0)[0x34 / 4])(state, nullptr);
                self->field_0280 = 1;
            }
        }
        return result;
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    GameBddLoader_ResetPending(globalState, nullptr);

    state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    int entry = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, state, 0)[0x20 / 4])(state, nullptr);

    if (entry == 1229730614)
    {
        globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = GameBddLoader_DispatchMenuRequest(
            globalState, nullptr, 1095053622, 1, 12, 0);
        self->field_0280 = 7;
    }
    else if (entry == -1)
    {
        return MenuAccounts_FallbackToList(
            reinterpret_cast<char*>(self), nullptr);
    }
    else
    {
        globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = GameBddLoader_DispatchMenuRequest(
            globalState, nullptr, entry, 32, 1, 0);
        self->field_0280 = 3;
    }

    return result;
}

HOOK(0x0055CB10, S_0055c780_ProcessAccountSelection, "auto");
