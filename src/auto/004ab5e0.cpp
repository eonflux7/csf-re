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

// Callees of the original, bound by address.
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance, 0x00492720, (void* self, void*, int newStance));
CSF_FUNC(int, __fastcall, C_00834050_SetActionMode, 0x00492760, (void* self, void*, int newActionMode));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// Repeated pattern in the original: v = GetLinkedObjectValue(a2); base = v ? v - 4 : 0.
static uint8_t* AiMoveTasks_GetLinkedObjectBase(TAiState* a2)
{
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    return linkedObjectValue ? reinterpret_cast<uint8_t*>(linkedObjectValue - 4)
                             : nullptr;
}

static int __fastcall AiMoveTasks_ResetMoveStateAndSetCoverStance(void* self, void* /*edx*/, TAiState* a2)
{
    // Reset the move-state block at +0x690 of the linked object.
    uint8_t* moveStateBase = AiMoveTasks_GetLinkedObjectBase(a2);
    C_004D10F0_ResetFields(moveStateBase + 0x690, nullptr);

    // Set cover stance 3 on the sub-object at +0x2C.
    uint8_t* stanceBase = AiMoveTasks_GetLinkedObjectBase(a2);
    CoverAttack_SetStance(CSF_AT(void*, stanceBase, 0x2C), nullptr, 3);

    // Virtual slot 35 (+0x8C) on the sub-object at +0x2C with (0, 0, -1, 0).
    uint8_t* moveBase = AiMoveTasks_GetLinkedObjectBase(a2);
    void* moveObj = CSF_AT(void*, moveBase, 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, moveObj, 0)[0x8C / 4])(moveObj, nullptr, 0, 0, -1, 0);

    // Set action mode 1 on the sub-object at +0x2C.
    uint8_t* modeBase = AiMoveTasks_GetLinkedObjectBase(a2);
    void* modeObj = CSF_AT(void*, modeBase, 0x2C);
    C_00834050_SetActionMode(modeObj, nullptr, 1);

    // Forward to CoverAttack_SetState with this+4.
    return CoverAttack_SetState_4DC150(a2, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self) + 4));
}

HOOK(0x004AB5E0, AiMoveTasks_ResetMoveStateAndSetCoverStance, "auto");
