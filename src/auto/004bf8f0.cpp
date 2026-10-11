#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, UnnamedSetRangeParams_4BC3A0, 0x004BC3A0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __fastcall, AiCore_ApplyLinkedObjectParameters, 0x004BE750, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiCore_ConfigureLinkedObjectRange(int object)
{
    void* self = reinterpret_cast<void*>(object);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uintptr_t linkedBase = linkedValue
        ? static_cast<uintptr_t>(linkedValue) - 4
        : 0;

    void* controller = CSF_AT(void*, reinterpret_cast<void*>(linkedBase), 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, static_cast<int>(linkedBase + 0x440));

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    uintptr_t linkedParameters = linkedBase + 0x440;

    int linkedField = GetLinkedObjectField10(self, nullptr);
    void* rangeParameters = reinterpret_cast<void*>(linkedParameters + 0xCC);
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(linkedField), 0x1C) <= 1u)
        return UnnamedSetRangeParams_4BC3A0(rangeParameters, nullptr, 1.0f, 2.0f);

    UnnamedSetRangeParams_4BC3A0(rangeParameters, nullptr, 2.0f, 4.0f);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedBase = linkedValue ? static_cast<uintptr_t>(linkedValue) - 4 : 0;
    return AiCore_ApplyLinkedObjectParameters(
        reinterpret_cast<void*>(linkedBase), nullptr,
        static_cast<int>(linkedParameters));
}

HOOK(0x004BF8F0, AiCore_ConfigureLinkedObjectRange, "auto");
