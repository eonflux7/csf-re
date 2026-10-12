#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(char*, __cdecl, AiDistracted_QueryLinkedStatusText, 0x007613E0, (int a1));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));

static const char* __fastcall AiDistracted_GetBaseStateName(TDudoso* self, void* /*edx*/, TAiState* a2)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    uint8_t* linkedBase = linkedValue ? (uint8_t*)(uintptr_t)(linkedValue - 4) : nullptr;
    uint8_t* linkedOwner = CSF_AT(uint8_t*, linkedBase, 0x2C);
    uint8_t* linkedSubobject = CSF_AT(uint8_t*, linkedOwner, 0x34);
    int stateKind = CSF_AT(int, linkedSubobject, 0x2FC);

    if (stateKind == 0)
    {
        int subAddr = (int)((uintptr_t)self + 4);
        return (const char*)(uintptr_t)CoverAttack_SetState_4DC150(a2, nullptr, subAddr);
    }
    if (stateKind == 1)
    {
        int subAddr = (int)((uintptr_t)self + 0x14);
        return (const char*)(uintptr_t)CoverAttack_SetState_4DC150(a2, nullptr, subAddr);
    }

    int linkedValue2 = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    uint8_t* linked2Base = linkedValue2 ? (uint8_t*)(uintptr_t)(linkedValue2 - 4) : nullptr;
    C_00833908* targetOwner = (C_00833908*)CSF_AT(void*, linked2Base, 0x2C);
    C_00833908_GetTargetName(targetOwner, nullptr);
    return AiDistracted_QueryLinkedStatusText(stateKind);
}

HOOK(0x004AA8C0, AiDistracted_GetBaseStateName, "auto");
