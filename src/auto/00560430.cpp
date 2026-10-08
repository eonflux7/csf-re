#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(void*, __cdecl, Globals_GetPrimaryContext, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, Globals_GetStateManager, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __fastcall, SessionSlot_ApplyValue_73F120, 0x0073F120, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SessionSlot_Reset_50E1E0, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x00506AC0, (int unused));

static void __fastcall CMenuMpSessions_ClearSessionSlotUnlessState3(CMenuMpSessions* self, void* /*edx*/)
{
    void* stateManager = Globals_GetStateManager();
    // Virtual slot 7 (offset 0x1C) on the state manager; compared against 3.
    int stateValue = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateManager, 0)[7])(stateManager, nullptr);

    if (stateValue != 3)
    {
        void* primaryContext = Globals_GetPrimaryContext();
        int savedValue = CSF_AT(int, primaryContext, 0x10);

        int globalDword = GetGlobalDword_73C020();
        SessionSlot_ApplyValue_73F120(reinterpret_cast<void*>(static_cast<uintptr_t>(globalDword)), nullptr, savedValue);

        void* contextForReset = Globals_GetPrimaryContext();
        SessionSlot_Reset_50E1E0(contextForReset, nullptr);

        Nullsub_506AC0(1);
        self->field_0014 = 0;
    }
}

HOOK(0x560430, CMenuMpSessions_ClearSessionSlotUnlessState3, "auto");
