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
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// Returns the sub-object base (linked value - 4), or nullptr when the linked value is 0.
static uint8_t* LinkedSubobjectBase(TAiState* a2)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    if (linkedValue)
        return reinterpret_cast<uint8_t*>(linkedValue - 4);
    return nullptr;
}

static int __fastcall AiMoveTasks_ResetMoveStateAndSetCoverStance(void* self, void* /*edx*/, TAiState* a2)
{
    // Reset the move-state block at +0x690 of the linked object.
    uint8_t* resetBase = LinkedSubobjectBase(a2);
    C_004D10F0_ResetFields(resetBase + 0x690, nullptr);

    // Linked lookup whose result is discarded by the original.
    GetLinkedObjectValue_4DB5C0(a2, nullptr);

    // Set stance 3 on the object at +0x2C.
    uint8_t* stanceBase = LinkedSubobjectBase(a2);
    void* stanceObj = CSF_AT(void*, stanceBase, 0x2C);
    CoverAttack_SetStance_492720(stanceObj, nullptr, 3);

    // Virtual call on the object at +0x2C, vtable slot 0x8C / 4 = 35, args (0, 0, -1, 0).
    uint8_t* vcallBase = LinkedSubobjectBase(a2);
    void* vcallObj = CSF_AT(void*, vcallBase, 0x2C);
    void** vtbl = CSF_AT(void**, vcallObj, 0);
    ((int(__fastcall*)(void*, void*, int, int, int, int))vtbl[0x8C / 4])(
        vcallObj, nullptr, 0, 0, -1, 0);

    // Set action mode 1 on the object at +0x2C.
    uint8_t* actionBase = LinkedSubobjectBase(a2);
    void* actionObj = CSF_AT(void*, actionBase, 0x2C);
    Character_SetActionMode(actionObj, nullptr, 1);

    // Forward to the cover-attack state setter with this + 4.
    int stateArg = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 4);
    return CoverAttack_SetState_4DC150(a2, nullptr, stateArg);
}

HOOK(0x4AB5E0, AiMoveTasks_ResetMoveStateAndSetCoverStance, "auto");
