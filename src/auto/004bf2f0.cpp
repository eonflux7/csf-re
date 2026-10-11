#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedObjectAction, 0x004BE750, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiCore_PrepareLinkedObjectAction(int contextValue)
{
    void* context = reinterpret_cast<void*>(contextValue);

    int linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    std::uintptr_t objectAddress =
        linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    void* object = reinterpret_cast<void*>(objectAddress);
    CSF_AT(std::uint32_t, object, 0x680) |= 1u;

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    object = reinterpret_cast<void*>(objectAddress);
    CoverAttack_SetStance_492720(CSF_AT(void*, object, 0x2C), nullptr, 2);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    object = reinterpret_cast<void*>(objectAddress);
    void* controller = CSF_AT(void*, object, 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    UnnamedApply_4DC180(context, nullptr, static_cast<int>(objectAddress + 0x440));

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    const std::uintptr_t actionAddress = objectAddress + 0x440;
    UnnamedSetRangeParams_4BC3A0(
        reinterpret_cast<void*>(objectAddress + 0x50C), nullptr, 1.0f, 4.0f);

    linkedValue = GetLinkedObjectValue_4DB5C0(context, nullptr);
    objectAddress = linkedValue ? static_cast<std::uintptr_t>(linkedValue) - 4 : 0;
    return AiCore_ApplyLinkedObjectAction(
        reinterpret_cast<void*>(objectAddress), nullptr,
        static_cast<int>(actionAddress));
}

HOOK(0x004BF2F0, AiCore_PrepareLinkedObjectAction, "auto");
