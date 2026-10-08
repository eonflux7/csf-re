#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __fastcall, MenuMpAccounts_SetAccountScreen, 0x0073F120, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, (int unused));

static void __fastcall CMenuMpSessions_ClearSessionSlotUnlessState3(CMenuMpSessions* self, void* /*edx*/)
{
    void* stateManager = GetGlobalState_6A6D70();
    // Virtual slot 7 (offset 0x1C) on the state manager; compared against 3.
    int stateValue = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateManager, 0)[7])(stateManager, nullptr);

    if (stateValue != 3)
    {
        void* primaryContext = GetGlobalState_50D190();
        int savedValue = CSF_AT(int, primaryContext, 0x10);

        int globalDword = GetGlobalDword_73C020();
        MenuMpAccounts_SetAccountScreen(reinterpret_cast<void*>(static_cast<uintptr_t>(globalDword)), nullptr, savedValue);

        void* contextForReset = GetGlobalState_50D190();
        GlobalState_ResetPending(contextForReset, nullptr);

        Nullsub_506AC0(1);
        self->field_0014 = 0;
    }
}

HOOK(0x560430, CMenuMpSessions_ClearSessionSlotUnlessState3, "auto");
