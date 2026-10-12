#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(char*, __cdecl, AiAlert_GetErrorDescription, 0x007613F0, (int a1));

static const char* __fastcall TAlerta_GetStateName(
    TAlerta* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
        : nullptr;

    void* target = CSF_AT(void*, linkedObject, 0x2C);
    void* targetState = CSF_AT(void*, target, 0x34);
    const int alertState = CSF_AT(int, targetState, 0x300);

    switch (alertState)
    {
    case 0:
        return reinterpret_cast<const char*>(CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 4)));
    case 1:
        return reinterpret_cast<const char*>(CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x18)));
    case 2:
        return reinterpret_cast<const char*>(CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x38)));
    case 3:
        return reinterpret_cast<const char*>(CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x5C)));
    case 4:
        return reinterpret_cast<const char*>(CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x70)));
    default:
        linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
        linkedObject = linkedValue
            ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
            : nullptr;
        C_00833908_GetTargetName(
            CSF_AT(C_00833908*, linkedObject, 0x2C), nullptr);
        return AiAlert_GetErrorDescription(alertState);
    }
}

HOOK(0x004AABC0, TAlerta_GetStateName, "auto");
