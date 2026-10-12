#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxCollisionShapes_GrowStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newStorage = Mem_Alloc(32u * capacity);
    int result = 0;

    if (newStorage &&
        static_cast<std::int32_t>(capacity - 1u) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = capacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x14) = 0;
            CSF_AT(std::uint32_t, entry, 0x18) = 0;
            CSF_AT(std::uint32_t, entry, 0x1C) = 0;
            CSF_AT(std::int32_t, entry, 0x00) = -1;
            CSF_AT(std::int32_t, entry, 0x04) = -1;
            CSF_AT(std::int32_t, entry, 0x08) = -1;
            CSF_AT(std::int32_t, entry, 0x0C) = -1;
            CSF_AT(std::int32_t, entry, 0x10) = -1;
            entry += 32;
        }
        while (--remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    if (count > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x00), 32u * count);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (oldStorage)
    {
        result = Mem_Free(oldStorage);
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newStorage;
    return result;
}

HOOK(0x006CFB20, GfxCollisionShapes_GrowStorage, "auto");
