#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

static int __stdcall AiCore_ResetLinkedObjectState(int objectAddress)
{
    void* object = reinterpret_cast<void*>(objectAddress);

    int linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    uint8_t* linked = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    CSF_AT(uint32_t, linked, 0x6DC) = 0;

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linked = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    void* controller = CSF_AT(void*, linked, 0x2C);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, controller, 0)[0x8C / 4])(
            controller, nullptr, 0, 0, -1, 0);

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linked = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    UnnamedApply_4DC180(
        object, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(linked + 0x440)));

    linkedValue = GetLinkedObjectValue_4DB5C0(object, nullptr);
    linked = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    auto* array = reinterpret_cast<S_0041e5f0*>(linked + 0x50C);
    if (array->field_0008 >= array->field_0004)
    {
        // Address alias identified as a vtable reference in the packet.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            array, nullptr, static_cast<int>(array->field_000c));
    }

    uint32_t count = array->field_0008;
    uint32_t* entry =
        reinterpret_cast<uint32_t*>(array->field_0000) + count;
    uint32_t newCount = count + 1;
    array->field_0008 = newCount;
    *entry = 0;
    return static_cast<int>(newCount);
}

HOOK(0x004BFCC0, AiCore_ResetLinkedObjectState, "auto");
