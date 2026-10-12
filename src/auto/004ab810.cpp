#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

static int __fastcall TMovilAInteractivo_2_InitializeInteraction(
    TMovilAInteractivo_2* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    uintptr_t linkedOwner = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    C_004D10F0_ResetFields(
        reinterpret_cast<void*>(linkedOwner + 0x690), nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedOwner = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    CoverAttack_SetStance_492720(
        reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(linkedOwner), 0x2C)),
        nullptr, 3);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedOwner = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    void* actor = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(linkedOwner), 0x2C));
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, actor, 0)[0x8C / 4])(
            actor, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedOwner = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    const bool hasNonNegativeField = static_cast<uint8_t>(
        IsField1BCNonNegative(
            reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(linkedOwner), 0x2C)),
            nullptr)) != 0;

    if (hasNonNegativeField)
    {
        return CoverAttack_SetState_4DC150(
            state, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10));
    }

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
}

HOOK(0x004AB810, TMovilAInteractivo_2_InitializeInteraction, "auto");
