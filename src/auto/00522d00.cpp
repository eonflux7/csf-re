#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall AudioSoundManager_GrowSoundEntryStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newStorage = Mem_Alloc(12u * capacity);
    int result = 0;

    if (newStorage &&
        static_cast<std::int32_t>(capacity - 1u) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = capacity;
        do
        {
            CSF_AT(std::uint8_t, entry, 0x0) = 0;
            CSF_AT(std::uint32_t, entry, 0x8) = 0;
            CSF_AT(float, entry, 0x4) = -1.0f;
            entry += 12;
        } while (--remaining);
    }

    const std::uint32_t entryCount = CSF_AT(std::uint32_t, self, 0x8);
    if (entryCount > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x0),
                    12u * entryCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    if (oldStorage)
    {
        result = Mem_Free(oldStorage);
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newStorage;
    return result;
}

HOOK(0x00522D00, AudioSoundManager_GrowSoundEntryStorage, "auto");
