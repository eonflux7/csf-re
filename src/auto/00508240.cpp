#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, AppMain_DestroyElementArray, 0x00508180, (void* self, void* edx, char a2));

static int __fastcall S_00508240_GrowCapacity(
    S_00508240* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(56u * capacity + 4u));
    std::uint8_t* elements = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = capacity;
        elements = allocation + 4;

        if (static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            std::uint8_t* element = elements;
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, element, 0x08) = 0;
                CSF_AT(std::uint32_t, element, 0x0C) = 0;
                CSF_AT(std::uint32_t, element, 0x10) = 0;
                CSF_AT(float, element, 0x14) = 1.0f;
                CSF_AT(std::uint32_t, element, 0x34) = 7;
                CSF_AT(std::uint32_t, element, 0x30) = 0;
                CSF_AT(std::uint16_t, element, 0x20) = 0;
                element += 56;
            } while (--remaining);
        }
    }

    const std::uint32_t count = self->field_0008;
    if (count)
    {
        std::memcpy(elements, reinterpret_cast<const void*>(self->field_0000),
                    56u * count);
    }

    const std::uint32_t oldElements = self->field_0000;
    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    int result;
    if (oldElements)
    {
        result = AppMain_DestroyElementArray(
            reinterpret_cast<void*>(oldElements), nullptr, 3);
        self->field_0000 = 0;
    }
    else
    {
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(elements));
    }

    self->field_0000 = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(elements));
    return result;
}

HOOK(0x00508240, S_00508240_GrowCapacity, "auto");
