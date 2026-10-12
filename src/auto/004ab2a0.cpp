#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(char*, __cdecl, AiAlert_LookupDescription, 0x00761400, (int a1));

static const char* __fastcall TCombate_GetName(
    TCombate* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* linkedObject = reinterpret_cast<void*>(
        linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0u);

    void* target = CSF_AT(void*, linkedObject, 0x2C);
    void* combatData = CSF_AT(void*, target, 0x34);
    int combatState = CSF_AT(int, combatData, 0x304);
    auto* members = reinterpret_cast<uint8_t*>(self);

    switch (combatState)
    {
    case 0:
        return reinterpret_cast<const char*>(
            CoverAttack_SetState_4DC150(
                state, nullptr, reinterpret_cast<int>(members + 4)));
    case 1:
        return reinterpret_cast<const char*>(
            CoverAttack_SetState_4DC150(
                state, nullptr, reinterpret_cast<int>(members + 0x24)));
    case 2:
        return reinterpret_cast<const char*>(
            CoverAttack_SetState_4DC150(
                state, nullptr, reinterpret_cast<int>(members + 0x30)));
    case 3:
        return reinterpret_cast<const char*>(
            CoverAttack_SetState_4DC150(
                state, nullptr, reinterpret_cast<int>(members + 0x48)));
    case 4:
        return reinterpret_cast<const char*>(
            CoverAttack_SetState_4DC150(
                state, nullptr, reinterpret_cast<int>(members + 0x54)));
    case 5:
        return reinterpret_cast<const char*>(
            CoverAttack_SetState_4DC150(
                state, nullptr, reinterpret_cast<int>(members + 0x78)));
    default:
        linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
        linkedObject = reinterpret_cast<void*>(
            linkedValue ? static_cast<uint32_t>(linkedValue) - 4 : 0u);
        C_00833908_GetTargetName(
            CSF_AT(C_00833908*, linkedObject, 0x2C), nullptr);
        return AiAlert_LookupDescription(combatState);
    }
}

HOOK(0x004AB2A0, TCombate_GetName, "auto");
