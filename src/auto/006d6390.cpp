#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// The first three parameters occupy EAX, EDX and ECX. The remaining
// five are the callback's ordinary, caller-cleaned stack arguments.
typedef int (__cdecl *SectorListCallback)(
    int, int, int, int, int, int, int, std::uint32_t*)
    __attribute__((regparm(3)));

CSF_GLOBAL(SectorListCallback, g_OriginalSectorListCallback, 0x008BB704);

CSF_FUNC(int, __cdecl, RpWorldSectorGetWorld, 0x0058D1A0, (unsigned int a1));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));

static int __cdecl Gfx2d_FilterWorldSectorEntries(
    int a1, int a2, int a3, int a4, std::uint32_t* entries)
{
    const int result = g_OriginalSectorListCallback(
        a1, a2, a3, a1, a2, a3, a4, entries);
    if (!result)
        return result;

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* renderHandle = reinterpret_cast<void*>(
        RenderHandle_FromObject(globalObject, nullptr));

    std::int32_t index = static_cast<std::int32_t>(entries[1] - 1);
    const std::uint32_t visibilityMask =
        CSF_AT(std::uint32_t, renderHandle, 0x20);

    bool removeEntry = false;
    std::uint16_t previousObjectIndex = static_cast<std::uint16_t>(-1);
    if (index < 0)
        return result;

    std::uint32_t entryOffset = 76 * static_cast<std::uint32_t>(index);
    do
    {
        auto* base = reinterpret_cast<std::uint8_t*>(entries[0]);
        void* sector = CSF_AT(void*, base + entryOffset, 0x40);
        auto* sectorRecords = CSF_AT(std::uint8_t*, sector, 0x4);
        const std::uint32_t recordIndex =
            CSF_AT(std::uint32_t, base + entryOffset, 0x44);
        const std::uint16_t objectIndex =
            CSF_AT(std::uint8_t, sectorRecords + 8 * recordIndex, 0x6);

        if (objectIndex != previousObjectIndex)
        {
            void* world = reinterpret_cast<void*>(
                RpWorldSectorGetWorld(reinterpret_cast<unsigned int>(sector)));
            auto* objectIndices = CSF_AT(int*, world, 0x10);
            void* sectorData = reinterpret_cast<void*>(
                GfxObjectProps_GetObjectSectorData(objectIndices[objectIndex]));

            removeEntry = sectorData &&
                (visibilityMask & CSF_AT(std::uint32_t, sectorData, 0xC)) == 0;
            previousObjectIndex = objectIndex;
        }

        if (removeEntry)
        {
            const std::int32_t count = static_cast<std::int32_t>(entries[1]);
            if (index < static_cast<std::int32_t>(
                    static_cast<std::uint32_t>(count) - 1))
            {
                auto* currentBase = reinterpret_cast<std::uint8_t*>(entries[0]);
                std::memcpy(
                    currentBase + entryOffset,
                    currentBase + 76 * static_cast<std::uint32_t>(count) - 76,
                    0x4C);
            }
            --entries[1];
        }

        --index;
        entryOffset -= 76;
    }
    while (index >= 0);

    return result;
}

HOOK(0x006D6390, Gfx2d_FilterWorldSectorEntries, "auto");
