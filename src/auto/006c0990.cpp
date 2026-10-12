#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxFrames_GrowFrameArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newStorage = Mem_Alloc(
        static_cast<std::size_t>(16u * capacity));

    if (newStorage &&
        static_cast<std::int32_t>(capacity - 1u) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = capacity;
        do
        {
            CSF_AT(std::uint32_t, record, 0x0) = 0;
            CSF_AT(std::uint32_t, record, 0x4) = 0;
            CSF_AT(std::uint32_t, record, 0x8) = 0;
            CSF_AT(std::uint32_t, record, 0xC) = 0;
            record += 16;
            --remaining;
        } while (remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount > 0)
    {
        std::memcpy(
            newStorage,
            CSF_AT(void*, self, 0x0),
            static_cast<std::size_t>(16u * usedCount));
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    int result;
    if (oldStorage)
    {
        result = Mem_Free(oldStorage);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    else
    {
        result = static_cast<int>(
            reinterpret_cast<std::uintptr_t>(newStorage));
    }

    CSF_AT(void*, self, 0x0) = newStorage;
    return result;
}

HOOK(0x006C0990, GfxFrames_GrowFrameArray, "auto");
