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

CSF_FUNC(int, __fastcall, SetEnableFlags, 0x004762F0, (void* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateWeaponActionState, 0x004849A0, (CCharActionFsm* self, void*));
CSF_FUNC(void, __fastcall, C_00833b88_RequestPrimaryAction, 0x00495270, (C_00833b88* self, void*));
CSF_FUNC(bool, __fastcall, C_008342e8_IsContextActionAvailable, 0x00495730, (C_008342e8* self, void*));

// Search-sweep order state machine step. When the order is in one of the
// active states (2, 3 or 4), refresh the weapon action state if no context
// action is available, enable the entry selected by the FSM's secondary
// index, request the primary action, and clear the order state back to idle.
static int __fastcall C_0083ab58_ProcessSearchOrderState(C_0083ab58* self, void* /*edx*/)
{
    int currentState = self->field_0040;
    if (currentState == 3 || currentState == 4 || currentState == 2) {
        C_008342e8* actionHost = reinterpret_cast<C_008342e8*>(self->field_0014);
        CCharActionFsm* fsm = reinterpret_cast<CCharActionFsm*>(actionHost->field_0044);
        if (!C_008342e8_IsContextActionAvailable(actionHost, nullptr)) {
            CCharActionFsm_UpdateWeaponActionState(fsm, nullptr);
        }

        // Slot 63 (+0xFC): GetSecondaryCount, no arguments.
        int secondaryIndex = ((int(__fastcall*)(void*, void*))CSF_AT(void**, fsm, 0)[63])(fsm, nullptr);
        // Slot 50 (+0xC8): GetIndexedEntry(index).
        int enabledEntry = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, fsm, 0)[50])(fsm, nullptr, secondaryIndex);
        if (enabledEntry) {
            SetEnableFlags(reinterpret_cast<void*>(enabledEntry), nullptr);
        }

        C_00833b88_RequestPrimaryAction(reinterpret_cast<C_00833b88*>(self->field_0014), nullptr);
        self->field_0048 = static_cast<uint32_t>(-1);
        self->field_004c = static_cast<uint32_t>(-1);
        self->field_0040 = 0;
        return -1;
    }
    return currentState;
}

HOOK(0x004CD740, C_0083ab58_ProcessSearchOrderState, "auto");
