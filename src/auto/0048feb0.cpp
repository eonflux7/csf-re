#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

static int __fastcall Game_AppendArrayValue(void* self, void* /*edx*/, int value)
{
    auto* array = reinterpret_cast<S_0041e5f0*>(
        static_cast<std::uint8_t*>(self) + 0x9C);

    if (CSF_AT(std::uint32_t, self, 0xA4) >=
        CSF_AT(std::uint32_t, self, 0xA0))
    {
        S_0041e5f0_GrowArrayCapacity(
            array, nullptr, CSF_AT(int, self, 0xA8));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0xA4);
    int* destination = CSF_AT(int*, self, 0x9C) + count;
    CSF_AT(std::uint32_t, self, 0xA4) = count + 1;
    *destination = value;
    return value;
}

HOOK(0x0048FEB0, Game_AppendArrayValue, "auto");
