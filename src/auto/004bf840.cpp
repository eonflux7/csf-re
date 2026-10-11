#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedObjectState, 0x004BE750, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiCore_InitializeLinkedObjectRanges(int object)
{
    void* owner = reinterpret_cast<void*>(static_cast<std::uintptr_t>(object));

    int linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    std::uint32_t linkedObjectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    void* linkedObject = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(linkedObjectAddress));
    void* controller = CSF_AT(void*, linkedObject, 0x2C);

    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    linkedObjectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    UnnamedApply_4DC180(
        owner, nullptr, static_cast<int>(linkedObjectAddress + 1088));

    linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    linkedObjectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    const int parametersAddress =
        static_cast<int>(linkedObjectAddress + 1088);

    UnnamedSetRangeParams_4BC3A0(
        reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(linkedObjectAddress + 1292)),
        nullptr, 2.0f, 4.0f);

    linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    linkedObjectAddress =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    return AiCore_ApplyLinkedObjectState(
        reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(linkedObjectAddress)),
        nullptr, parametersAddress);
}

HOOK(0x004BF840, AiCore_InitializeLinkedObjectRanges, "auto");
