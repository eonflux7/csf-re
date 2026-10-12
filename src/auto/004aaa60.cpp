#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall TMovilAPunto_EnterMovementState(
    TMovilAPunto* self, void* /*edx*/, TAiState* state)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    std::uintptr_t ownerAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    void* firstGlobal = reinterpret_cast<void*>(
        static_cast<std::uint32_t>(GetGlobalValue_8BC1E0()));
    void* secondGlobal = reinterpret_cast<void*>(
        static_cast<std::uint32_t>(GetGlobalValue_8BC1E0()));

    float transitionDuration = CSF_AT(float, secondGlobal, 0xA90);
    int fullDurationBits = CSF_AT(int, firstGlobal, 0xA90);
    transitionDuration *= g_HalfDurationScale;

    int transitionDurationBits;
    std::memcpy(&transitionDurationBits, &transitionDuration,
                sizeof(transitionDurationBits));
    AiCoverAttackTasks_SetCoverTransition(
        reinterpret_cast<void*>(ownerAddress + 0x690), nullptr,
        transitionDurationBits, fullDurationBits);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    CoverAttack_SetStance_492720(
        CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C),
        nullptr, 1);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    void* character =
        CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, character, 0)[0x8C / 4])(
            character, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    ownerAddress = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    Character_SetActionMode(
        CSF_AT(void*, reinterpret_cast<void*>(ownerAddress), 0x2C),
        nullptr, 0);

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(
            reinterpret_cast<std::uint8_t*>(self) + 4)));
}

HOOK(0x004AAA60, TMovilAPunto_EnterMovementState, "auto");
