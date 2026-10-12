#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxCollisionShapes_GrowElementStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newStorage = Mem_Alloc(newCapacity * 56u);
    if (newStorage &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* element = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, element, 0x04) = 0;
            CSF_AT(std::uint32_t, element, 0x08) = 0;
            CSF_AT(std::uint32_t, element, 0x0C) = 0;
            CSF_AT(std::uint32_t, element, 0x10) = 0;
            CSF_AT(std::uint32_t, element, 0x14) = 0;
            CSF_AT(std::uint32_t, element, 0x18) = 0;
            CSF_AT(std::uint32_t, element, 0x1C) = 0;
            CSF_AT(std::int32_t, element, 0x20) = -1;
            CSF_AT(std::int32_t, element, 0x24) = -1;
            CSF_AT(std::int32_t, element, 0x28) = -1;
            CSF_AT(std::int32_t, element, 0x2C) = -1;
            CSF_AT(std::int32_t, element, 0x30) = -1;
            CSF_AT(std::int32_t, element, 0x34) = -1;
            element += 56;
        } while (--remaining);
    }

    const std::uint32_t elementCount = CSF_AT(std::uint32_t, self, 0x08);
    if (elementCount > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x00),
                    elementCount * 56u);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    int result;
    if (oldStorage)
    {
        result = Mem_Free(oldStorage);
        CSF_AT(void*, self, 0x00) = nullptr;
    }
    else
    {
        result = static_cast<int>(
            reinterpret_cast<std::intptr_t>(newStorage));
    }

    CSF_AT(void*, self, 0x00) = newStorage;
    return result;
}

HOOK(0x006CFA50, GfxCollisionShapes_GrowElementStorage, "auto");
