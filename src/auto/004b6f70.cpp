#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_004af750_CheckSlotPosition, 0x004B2EE0, (S_004af750* self, void* , float* position, int slot));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

static int __fastcall TFncAtacarDesdeParapeto_SelectAttackState(
    TFncAtacarDesdeParapeto* self, void* /*edx*/, TAiState* state)
{
    const int firstLinkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    auto* firstObject = reinterpret_cast<std::uint8_t*>(
        firstLinkedValue ? static_cast<std::uint32_t>(firstLinkedValue) - 4 : 0);

    const int secondLinkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    auto* coverObject = reinterpret_cast<std::uint8_t*>(
        secondLinkedValue ? static_cast<std::uint32_t>(secondLinkedValue) - 4 : 0);

    auto* slot = firstObject + 0x628;
    if (CSF_AT(int, firstObject, 0x630) < 0)
        slot = firstObject + 0x610;

    if (CSF_AT(std::uint32_t, coverObject, 0x6C8) != 0 &&
        CSF_AT(int, coverObject, 0x6C0) != -1 &&
        CSF_AT(int, coverObject, 0x6C4) != -1 &&
        S_004af750_CheckSlotPosition(
            reinterpret_cast<S_004af750*>(coverObject),
            nullptr,
            reinterpret_cast<float*>(coverObject + 0x6B4),
            static_cast<int>(reinterpret_cast<std::intptr_t>(slot))))
    {
        return CoverAttack_SetState_4DC150(
            state, nullptr,
            static_cast<int>(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<std::uint8_t*>(self) + 8)));
    }

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        static_cast<int>(reinterpret_cast<std::intptr_t>(
            reinterpret_cast<std::uint8_t*>(self) + 0xC)));
}

HOOK(0x004B6F70, TFncAtacarDesdeParapeto_SelectAttackState, "auto");
