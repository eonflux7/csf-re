#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, Environment_TidyStringOrArray, 0x006BDA20, (void* self, void* , char flags));

static std::uint32_t* __fastcall S_006be440_GrowStorage(
    S_006be440* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    const std::uint32_t allocationSize = 56 * newCapacity + 4;
    auto* allocation = static_cast<std::uint8_t*>(Mem_Alloc(allocationSize));
    std::uint32_t* newStorage = nullptr;

    if (allocation)
    {
        newStorage = reinterpret_cast<std::uint32_t*>(allocation + 4);
        CSF_AT(std::uint32_t, allocation, 0) = newCapacity;

        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            auto* entry = reinterpret_cast<std::uint8_t*>(newStorage);
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(std::uint32_t, entry, 0x00) = 0;
                CSF_AT(std::int32_t, entry, 0x04) = -1;
                CSF_AT(std::uint32_t, entry, 0x20) = 15;
                CSF_AT(std::uint32_t, entry, 0x1C) = 0;
                CSF_AT(std::uint8_t, entry, 0x0C) = 0;
                CSF_AT(std::uint32_t, entry, 0x24) = 0;
                CSF_AT(std::uint32_t, entry, 0x28) = 0;
                CSF_AT(std::uint32_t, entry, 0x2C) = 0;
                CSF_AT(std::uint32_t, entry, 0x30) = 0;
                CSF_AT(std::uint32_t, entry, 0x34) = 0;
                entry += 56;
            } while (--remaining);
        }
    }

    if (self->field_0008 != 0)
    {
        const std::uint32_t copySize = 56 * self->field_0008;
        std::memcpy(newStorage,
                    reinterpret_cast<const void*>(self->field_0000),
                    copySize);
    }

    void* oldStorage = reinterpret_cast<void*>(self->field_0000);
    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    if (!oldStorage)
    {
        self->field_0000 = reinterpret_cast<std::uintptr_t>(newStorage);
        return newStorage;
    }

    const int cleanupResult = Environment_TidyStringOrArray(oldStorage, nullptr, 3);
    self->field_0000 = 0;
    self->field_0000 = reinterpret_cast<std::uintptr_t>(newStorage);
    return reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(cleanupResult));
}

HOOK(0x006BE440, S_006be440_GrowStorage, "auto");
