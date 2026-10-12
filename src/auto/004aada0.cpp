#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall TMovilAPunto_2_EnterMovementState(
    TMovilAPunto_2* self, void* /*edx*/, TAiState* state)
{
    int linkedObject = GetLinkedObjectValue_4DB5C0(state, nullptr);
    std::uint32_t ownerAddress =
        linkedObject ? static_cast<std::uint32_t>(linkedObject) - 4 : 0;

    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(ownerAddress + 0x690), nullptr);

    linkedObject = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress =
        linkedObject ? static_cast<std::uint32_t>(linkedObject) - 4 : 0;

    void* durationSource =
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* scaledDurationSource =
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    float scaledDuration = CSF_AT(float, scaledDurationSource, 0xA94);
    int durationBits = CSF_AT(int, durationSource, 0xA94);
    scaledDuration *= g_HalfDurationScale;

    int scaledDurationBits;
    std::memcpy(&scaledDurationBits, &scaledDuration, sizeof(scaledDurationBits));
    AiCoverAttackTasks_SetCoverTransition(
        reinterpret_cast<void*>(ownerAddress + 0x690),
        nullptr, scaledDurationBits, durationBits);

    linkedObject = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress =
        linkedObject ? static_cast<std::uint32_t>(linkedObject) - 4 : 0;
    void* character =
        CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    CoverAttack_SetStance_492720(character, nullptr, 2);

    linkedObject = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress =
        linkedObject ? static_cast<std::uint32_t>(linkedObject) - 4 : 0;
    character = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, character, 0)[0x8C / 4])(
            character, nullptr, 0, 0, -1, 0);

    linkedObject = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress =
        linkedObject ? static_cast<std::uint32_t>(linkedObject) - 4 : 0;
    character = CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    Character_SetActionMode(character, nullptr, 0);

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self) + 4));
}

HOOK(0x004AADA0, TMovilAPunto_2_EnterMovementState, "auto");
