#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, AiCore_ActivateUnassignedCoverState, 0x004AA990,
         (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __stdcall, AiCore_ActivateAssignedCoverState, 0x004BA3C0,
         (void* a1, char a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0,
         (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150,
         (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall AiCore_SelectCoverAttackState(
    void* self, void* /*edx*/, void* aiState)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    auto* linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(
              static_cast<std::uint32_t>(linkedValue) - 4)
        : nullptr;

    void* firstGlobal =
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* secondGlobal =
        reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    float duration = CSF_AT(float, secondGlobal, 0xA90);
    int originalDurationBits = CSF_AT(int, firstGlobal, 0xA90);
    float halfDuration = duration * g_HalfDurationScale;
    int halfDurationBits;
    std::memcpy(&halfDurationBits, &halfDuration, sizeof(halfDurationBits));

    AiCoverAttackTasks_SetCoverTransition(
        linkedObject + 0x690, nullptr,
        halfDurationBits, originalDurationBits);

    linkedValue = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<std::uint8_t*>(
              static_cast<std::uint32_t>(linkedValue) - 4)
        : nullptr;

    if (CSF_AT(int, linkedObject, 0x608) == -1 &&
        CSF_AT(int, linkedObject, 0x60C) == -1)
    {
        void* selectedState = static_cast<std::uint8_t*>(self) + 8;
        CoverAttack_SetState_4DC150(
            aiState, nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(selectedState)));

        void* provider = GetCachedObjectProvider(aiState, nullptr);
        return AiCore_ActivateUnassignedCoverState(
            selectedState, nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(provider)), 1);
    }

    void* selectedState = static_cast<std::uint8_t*>(self) + 4;
    CoverAttack_SetState_4DC150(
        aiState, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(selectedState)));

    void* provider = GetCachedObjectProvider(aiState, nullptr);
    return AiCore_ActivateAssignedCoverState(provider, 1);
}

HOOK(0x004BEC20, AiCore_SelectCoverAttackState, "auto");
