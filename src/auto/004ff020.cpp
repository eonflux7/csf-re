#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __stdcall, SceneLoader_ConstructArrayElements, 0x004011C0,
         (void* first, int elementSize, int count,
          int (__thiscall* constructor)(void*)));

static std::uint32_t* __fastcall SceneLoader_GrowEntryArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t totalCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* replacement = static_cast<std::uint8_t*>(
        Mem_Alloc(68 * totalCapacity));

    if (replacement &&
        static_cast<std::int32_t>(totalCapacity - 1) >= 0)
    {
        auto* entry = replacement;
        std::uint32_t remaining = totalCapacity;
        do
        {
            SceneLoader_ConstructArrayElements(
                entry + 0x08, 12, 4,
                reinterpret_cast<int (__thiscall*)(void*)>(
                    CSF_ADDR_FN(0x0068E2B0)));

            CSF_AT(std::uint8_t, entry, 0x00) = 0;
            CSF_AT(std::int32_t, entry, 0x38) = -1;
            CSF_AT(std::int32_t, entry, 0x3C) = -1;
            CSF_AT(std::uint32_t, entry, 0x40) = 0;
            entry += 68;
            --remaining;
        } while (remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount != 0)
    {
        std::memcpy(replacement, CSF_AT(void*, self, 0x00),
                    68 * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldEntries = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (oldEntries)
    {
        const int result = Mem_Free(oldEntries);
        CSF_AT(void*, self, 0x00) = nullptr;
        CSF_AT(void*, self, 0x00) = replacement;
        return reinterpret_cast<std::uint32_t*>(result);
    }

    CSF_AT(void*, self, 0x00) = replacement;
    return reinterpret_cast<std::uint32_t*>(replacement);
}

HOOK(0x004FF020, SceneLoader_GrowEntryArray, "auto");
