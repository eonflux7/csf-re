#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees, bound by address.
CSF_FUNC(void, __stdcall, HudWidget_PushStateEntry, 0x0042DA20, (int a1, int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentState, 0x004834F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));

// Virtual slot signatures on the FSM object (slot = byte offset / 4).
using FsmGetIntSlot = int(__fastcall*)(void* self, void* edx);
using FsmMapIntSlot = int(__fastcall*)(void* self, void* edx, int arg);

static void __stdcall HudWidget_RefreshFsmStateSlots(C_00833908* self)
{
    if (!Hud_IsSuspended(self, nullptr))
    {
        void* fsm = reinterpret_cast<void*>(self->field_0044);
        void** fsmVtbl = CSF_AT(void**, fsm, 0);

        // Slot 0xFC: current state index; -1 means no active state.
        if (((FsmGetIntSlot)fsmVtbl[0xFC / 4])(fsm, nullptr) != -1)
        {
            fsmVtbl = CSF_AT(void**, fsm, 0);
            int stateIndex = ((FsmGetIntSlot)fsmVtbl[0xFC / 4])(fsm, nullptr);
            int mappedValue = ((FsmMapIntSlot)fsmVtbl[0xC8 / 4])(fsm, nullptr, stateIndex);

            HudWidget_PushStateEntry(reinterpret_cast<int>(self), mappedValue);

            if (CharActionFsm_GetCurrentState(fsm, nullptr) == 10)
            {
                HudWidget_PushStateEntry(reinterpret_cast<int>(self), CSF_AT(int, fsm, 0x9C));  // the FSM's +0x9C
            }
        }
    }
}

HOOK(0x0042DAB0, HudWidget_RefreshFsmStateSlots, "auto");
