#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

static int __fastcall S_0041e5f0_AppendVector(
    S_0041e5f0* self, void* /*edx*/, uint32_t* vector)
{
    auto* array = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 0x10);

    uint32_t value = 0;
    for (int component = 0; component < 3; ++component)
    {
        if (array->field_0008 >= array->field_0004)
        {
            S_0041e5f0_GrowArrayCapacity(
                array, nullptr, static_cast<int>(array->field_000c));
        }

        const uint32_t index = array->field_0008;
        auto* destination = reinterpret_cast<uint32_t*>(
            array->field_0000 + 4 * index);
        array->field_0008 = index + 1;
        value = vector[component];
        *destination = value;
    }

    return static_cast<int>(value);
}

HOOK(0x004B7150, S_0041e5f0_AppendVector, "auto");
