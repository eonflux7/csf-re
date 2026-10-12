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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, CCharActionFsm_GetSlot28ItemIfUsable, 0x00487260, (CCharActionFsm* self, void*));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void, __fastcall, C_00833908_SendCharacterEvent, 0x00525B30, (C_00833908* self, void*, int character, int eventType));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void, __fastcall, WeaponFirearms_SetTimer, 0x00476190, (void* self, void*, float value));

// Checks the slot-28 item of the action FSM; if its float at +0x10 is <= 0 it sends event 109,
// then sets the firearm timer from the global value at +0xAF0. Always tail-forwards to vtable slot 0xC4.
static hook::Tail __fastcall C_00833908_CheckSlot28TimerAndForward(void* self, const uint32_t* args)
{
    (void)args;
    C_00833908* obj = reinterpret_cast<C_00833908*>(self);

    int slotItem = CCharActionFsm_GetSlot28ItemIfUsable(reinterpret_cast<CCharActionFsm*>(obj->field_0044), nullptr);
    if (slotItem != 0)
    {
        if (CSF_AT(float, reinterpret_cast<uint8_t*>(slotItem), 0x10) <= g_fZero)
        {
            // Original pushes (0x6d, this) before calling Get_008b0678; the callee's
            // result becomes ecx (self) of the event sender, and the pushed values are its args.
            int eventHost = Get_008b0678();
            C_00833908_SendCharacterEvent(
                reinterpret_cast<C_00833908*>(eventHost),
                nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(obj)),
                109);
        }

        int globalValue = GetGlobalValue_8BC1E0();
        WeaponFirearms_SetTimer(reinterpret_cast<void*>(slotItem), nullptr,
                                CSF_AT(float, reinterpret_cast<uint8_t*>(globalValue), 0xAF0));
    }

    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xc4 / 4]);
}

HOOK_TAIL(0x00495160, C_00833908_CheckSlot28TimerAndForward, "game");
