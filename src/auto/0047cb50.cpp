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

// Callee: CharActionFsm_CopyParamsFromSource (rets=4)
CSF_FUNC(char, __fastcall, CharActionFsm_CopyParamsFromSource, 0x0047FAB0, (void* self, void* , int a2));

// Original: int __thiscall sub_47CB50(this, a2) ret 4
// Builds a 0x24-byte attack-parameter block on the stack from a2[0..2], then either
// hands it to the actor's FSM (when the FSM reports state 3) or to the actor's sub-object.
static int __fastcall AiAttackTasks_DispatchAttackParams(void* self, void* /*edx*/, const int* attackArgs)
{
    void* owner = CSF_AT(void*, self, 0x14);
    void* fsm = CSF_AT(void*, owner, 0x44);

    alignas(8) uint8_t paramBytes[0x24];
    uint8_t* params = paramBytes;

    CSF_AT(uint8_t, params, 0x20) = 1;                       // flag byte (overwritten below)
    CSF_AT(int, params, 0x00) = 1;
    CSF_AT(uint8_t, params, 0x22) = 1;
    CSF_AT(int, params, 0x0C) = -1;
    CSF_AT(int, params, 0x10) = -1;
    CSF_AT(int, params, 0x14) = attackArgs[0];
    CSF_AT(int, params, 0x18) = attackArgs[1];
    CSF_AT(int, params, 0x1C) = attackArgs[2];
    CSF_AT(void*, params, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(uint8_t, params, 0x21) = 0;
    CSF_AT(int, params, 0x04) = 3;

    void* owningSub = CSF_AT(void*, owner, 0x3C);
    CSF_AT(uint8_t, params, 0x20) = CSF_AT(uint8_t, owningSub, 0x4D);

    // fsm->vtbl[244 / 4]() == 3 ?
    int fsmState = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, fsm, 0)[0xF4 / 4])(fsm, nullptr);
    if (fsmState == 3)
    {
        return CharActionFsm_CopyParamsFromSource(fsm, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(params)));
    }

    // owner+4 is an embedded sub-object; call its vtbl[88 / 4](this, params)
    void* childObject = static_cast<uint8_t*>(owner) + 4;
    return reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(CSF_AT(void**, childObject, 0)[0x58 / 4])(
        childObject, nullptr, params);
}

HOOK(0x0047CB50, AiAttackTasks_DispatchAttackParams, "auto");
