#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall TMovilAInteractivo_EnterInteraction(
    TMovilAInteractivo* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    std::uintptr_t linkedBase =
        linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;

    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(linkedBase + 0x690), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedBase = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;

    const int firstGlobal = GetGlobalValue_8BC1E0();
    const int secondGlobal = GetGlobalValue_8BC1E0();
    const float duration = CSF_AT(
        float, reinterpret_cast<void*>(secondGlobal), 0xA94);
    const int fullDurationBits = CSF_AT(
        int, reinterpret_cast<void*>(firstGlobal), 0xA94);
    const float scaledDuration = duration * g_HalfDurationScale;
    int scaledDurationBits;
    std::memcpy(&scaledDurationBits, &scaledDuration, sizeof(scaledDurationBits));

    AiCoverAttackTasks_SetCoverTransition(
        reinterpret_cast<void*>(linkedBase + 0x690),
        nullptr, scaledDurationBits, fullDurationBits);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedBase = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    CoverAttack_SetStance_492720(
        CSF_AT(void*, reinterpret_cast<void*>(linkedBase), 0x2C), nullptr, 2);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedBase = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    void* actor = CSF_AT(void*, reinterpret_cast<void*>(linkedBase), 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedBase = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    if (static_cast<std::uint8_t>(IsField1BCNonNegative(
            CSF_AT(void*, reinterpret_cast<void*>(linkedBase), 0x2C), nullptr)))
    {
        return CoverAttack_SetState_4DC150(
            state, nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(
                reinterpret_cast<std::uint8_t*>(self) + 0x10)));
    }

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(
            reinterpret_cast<std::uint8_t*>(self) + 4)));
}

HOOK(0x004AB140, TMovilAInteractivo_EnterInteraction, "auto");
