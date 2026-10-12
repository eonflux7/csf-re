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

// Resolves the linked object for a2: (linked - 4), or null when no link exists.
// The caller then reads fields at base + offset exactly as the original does.
static uint8_t* ResolveLinkedBase(TAiState* a2)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    if (linkedValue)
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue - 4));
    return nullptr;
}

// Clears the per-task fields, sets stance 3 on the linked object, issues the
// -1 virtual command on it, then hands the task over to CoverAttack_SetState.
static int __fastcall AiMoveTasks_ResetFieldsAndSetState(void* self, void* /*edx*/, TAiState* a2)
{
    uint8_t* resetBase = ResolveLinkedBase(a2);
    C_004D10F0_ResetFields(resetBase + 0x690, nullptr);

    uint8_t* stanceBase = ResolveLinkedBase(a2);
    CoverAttack_SetStance_492720(CSF_AT(void*, stanceBase, 0x2c), nullptr, 3);

    uint8_t* commandBase = ResolveLinkedBase(a2);
    void* commandObj = CSF_AT(void*, commandBase, 0x2c);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, commandObj, 0)[0x8c / 4])(
        commandObj, nullptr, 0, 0, -1, 0);

    return CoverAttack_SetState_4DC150(a2, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self) + 4));
}

HOOK(0x4AB520, AiMoveTasks_ResetFieldsAndSetState, "auto");
