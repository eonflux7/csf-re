#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCore_UpdateObjectRange, 0x004BE750, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiCore_UpdateLinkedObjectRange(int objectAddress, int, int)
{
    void* object = reinterpret_cast<void*>(objectAddress);

    int linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    std::uint32_t linkedBase =
        linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;

    UnnamedApply_4DC180(object, nullptr, static_cast<int>(linkedBase + 1088));

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linkedBase = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    const std::uint32_t rangeAddress = linkedBase + 1088;

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linkedBase = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    void* linkedObject = reinterpret_cast<void*>(linkedBase);

    if (CSF_AT(int, linkedObject, 0x608) != -1 ||
        CSF_AT(int, linkedObject, 0x60C) != -1)
    {
        return UnnamedSetRangeParams_4BC3A0(
            reinterpret_cast<void*>(rangeAddress + 204), nullptr, 0.0f, 1.0f);
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linkedBase = linkedValue ? static_cast<std::uint32_t>(linkedValue) - 4 : 0;
    return AiCore_UpdateObjectRange(
        reinterpret_cast<void*>(linkedBase), nullptr,
        static_cast<int>(rangeAddress));
}

HOOK(0x004BEB70, AiCore_UpdateLinkedObjectRange, "auto");
