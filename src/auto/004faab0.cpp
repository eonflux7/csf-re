#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
static CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static void* __fastcall DevConsole_GrowCommandArray(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    const std::uintptr_t newStorageValue =
        static_cast<std::uintptr_t>(CoreMemory_AllocUntagged(312 * newCapacity));
    void* newStorage = reinterpret_cast<void*>(newStorageValue);

    const std::uint32_t entryCount = CSF_AT(std::uint32_t, self, 0x8);
    if (entryCount != 0)
        std::memcpy(newStorage, CSF_AT(void*, self, 0x0), 312 * entryCount);

    void* oldStorage = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = newCapacity;

    if (oldStorage != nullptr)
    {
        const int freeResult = CoreMemory_Free(oldStorage);
        CSF_AT(void*, self, 0x0) = nullptr;
        CSF_AT(void*, self, 0x0) = newStorage;
        return reinterpret_cast<void*>(static_cast<std::uintptr_t>(freeResult));
    }

    CSF_AT(void*, self, 0x0) = newStorage;
    return newStorageValue != 0 ? reinterpret_cast<void*>(newStorageValue) : nullptr;
}

HOOK(0x004FAAB0, DevConsole_GrowCommandArray, "auto");
