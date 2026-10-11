#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall RecordArray_GrowCapacity(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newStorage = Mem_Alloc(12u * newCapacity);
    if (newStorage &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::int32_t, record, 0x0) = -1;
            record += 12;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount != 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x0),
                    12u * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    const void* result = oldStorage;
    if (oldStorage)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldStorage)));
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newStorage;
    return result;
}

HOOK(0x00732D00, RecordArray_GrowCapacity, "auto");
