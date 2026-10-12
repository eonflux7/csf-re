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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// Calls GetLinkedObjectValue(a2) and returns (value - 4), or null when the value is 0.
static uint8_t* AiMoveTask_ResolveObjectBase(TAiState* a2) {
    int linkedValue = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    if (linkedValue == 0) {
        return nullptr;
    }
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));
}

// Resets the task's state, sets the stance to 3, invokes a virtual method on the
// linked object, and finally forwards to CoverAttack_SetState with this+4.
static int __fastcall AiMoveTask_ResetAndSetStance(void* self, void* /*edx*/, TAiState* a2) {
    uint8_t* base = AiMoveTask_ResolveObjectBase(a2);
    CSF_AT(uint8_t, base, 0x6B0) = 0;

    uint8_t* resetBase = AiMoveTask_ResolveObjectBase(a2);
    C_004D10F0_ResetFields(static_cast<void*>(resetBase + 0x690), nullptr);

    uint8_t* stanceBase = AiMoveTask_ResolveObjectBase(a2);
    CoverAttack_SetStance_492720(CSF_AT(void*, stanceBase, 0x2C), nullptr, 3);

    uint8_t* vcallBase = AiMoveTask_ResolveObjectBase(a2);
    void* target = CSF_AT(void*, vcallBase, 0x2C);
    using SlotFn = int(__fastcall*)(void* self, void* edx, int a, int b, int c, int d);
    SlotFn slotFn = reinterpret_cast<SlotFn>(CSF_AT(void**, target, 0)[0x8C / 4]);
    slotFn(target, nullptr, 0, 0, -1, 0);

    return CoverAttack_SetState_4DC150(a2, nullptr,
                                       reinterpret_cast<int>(static_cast<uint8_t*>(self) + 4));
}

HOOK(0x004AB680, AiMoveTask_ResetAndSetStance, "auto");
