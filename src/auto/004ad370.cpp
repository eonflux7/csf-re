#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance, 0x00492720, (void* self, void* , int newStance));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D900, 0x0050D900, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __stdcall AiOrder_ApplyStanceAndTransitionScale(void* order)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(order, nullptr);
    auto* owner = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    CoverAttack_SetStance(CSF_AT(void*, owner, 0x2C), nullptr, 3);

    linkedValue = GetLinkedObjectValue_4DB5C0(order, nullptr);
    owner = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    void* coverObject = CSF_AT(void*, owner, 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, coverObject, 0)[0x8C / 4])(
            coverObject, nullptr, 0, 0, -1, 0);

    float transitionScale = 1.0f;
    linkedValue = GetLinkedObjectValue_4DB5C0(order, nullptr);
    owner = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    coverObject = CSF_AT(void*, owner, 0x2C);
    if (CSF_AT(int, coverObject, 0x50) == 1)
    {
        int globalState = GetGlobalState_50D190();
        transitionScale = MenuSystem_GetTransitionScale_50D900(
            reinterpret_cast<void*>(globalState), nullptr);
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(order, nullptr);
    owner = linkedValue
        ? reinterpret_cast<std::uint8_t*>(linkedValue) - 4
        : nullptr;

    int result = GetGlobalValue_8BC1E0();
    CSF_AT(float, owner, 0x69C) =
        transitionScale * CSF_AT(float, reinterpret_cast<void*>(result), 0xAA8);
    return result;
}

HOOK(0x004AD370, AiOrder_ApplyStanceAndTransitionScale, "auto");
