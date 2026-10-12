#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall AiMoveTasks_DispatchStaticAction(
    void* self, void* /*edx*/, void* linkedObject, char action)
{
    CSF_AT(char, self, 0x4) = action;

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    auto* actor = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(
            linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4u : 0u));
    CSF_AT(std::uint32_t, actor, 0x680) |= 1u;

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    actor = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(
            linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4u : 0u));

    void* firstGlobal = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(
            static_cast<std::uint32_t>(GetGlobalValue_8BC1E0())));
    void* secondGlobal = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(
            static_cast<std::uint32_t>(GetGlobalValue_8BC1E0())));

    float duration = CSF_AT(float, secondGlobal, 0xA94);
    int originalDurationBits = CSF_AT(int, firstGlobal, 0xA94);
    duration *= g_HalfDurationScale;

    int scaledDurationBits;
    std::memcpy(&scaledDurationBits, &duration, sizeof(scaledDurationBits));
    AiCoverAttackTasks_SetCoverTransition(
        actor + 0x690, nullptr, scaledDurationBits, originalDurationBits);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    actor = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(
            linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4u : 0u));
    CoverAttack_SetStance_492720(CSF_AT(void*, actor, 0x2C), nullptr, 2);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    actor = reinterpret_cast<std::uint8_t*>(
        static_cast<std::uintptr_t>(
            linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4u : 0u));
    void* controlledObject = CSF_AT(void*, actor, 0x2C);

    return reinterpret_cast<int (__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controlledObject, 0)[0x8C / 4])(
            controlledObject, nullptr, 0, 0, -1, 0);
}

HOOK(0x004AACE0, AiMoveTasks_DispatchStaticAction, "auto");
