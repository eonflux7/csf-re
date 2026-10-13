#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, SpawnPoints_GrowEntryBuffer, 0x00551CF0, (void* self, void* , int extraEntries));
CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x0058CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));
CSF_FUNC(int, __cdecl, RwStreamRead, 0x006293E0, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall C_008605d8_ReadEnvironmentChunk(
    C_008605d8* self, void* /*edx*/, uint32_t* stream)
{
    alignas(8) uint8_t entriesBytes[0x10]{};
    auto& entryBuffer = CSF_AT(void*, entriesBytes, 0x0);
    auto& capacity = CSF_AT(uint32_t, entriesBytes, 0x4);
    auto& entryCount = CSF_AT(uint32_t, entriesBytes, 0x8);
    auto& growth = CSF_AT(int, entriesBytes, 0xC);
    growth = 10;

    alignas(8) uint8_t recordBytes[0xC]{};
    auto& recordCount = CSF_AT(uint32_t, recordBytes, 0x0);
    auto& recordData = CSF_AT(void*, recordBytes, 0x4);

    uint32_t groupCount = reinterpret_cast<uint32_t>(stream);
    RwStreamRead(stream, &groupCount, 4);

    uint32_t groupIndex = 0;
    if (groupCount)
    {
        do
        {
            RwStreamRead(stream, &recordCount, 4);
            if (recordCount)
            {
                recordData = Mem_Alloc(24 * recordCount);
                RwStreamRead(stream, recordData, 24 * recordCount);
            }
            else
            {
                recordData = nullptr;
            }

            if (entryCount >= capacity)
                SpawnPoints_GrowEntryBuffer(entriesBytes, nullptr, growth);

            auto* entry = static_cast<uint8_t*>(entryBuffer) + 8 * entryCount;
            ++entryCount;
            CSF_AT(uint32_t, entry, 0x0) = recordCount;
            CSF_AT(void*, entry, 0x4) = recordData;
            ++groupIndex;
        } while (groupIndex < groupCount);
    }

    const uint32_t world = self->field_0004;
    if (world)
    {
        RpWorldForAllWorldSectors(
            static_cast<int>(world),
            reinterpret_cast<int (__cdecl*)(int*, int)>(
                CSF_ADDR_FN(0x006B8500)),
            reinterpret_cast<int>(entriesBytes));
    }

    entryCount = 0;
    if (entryBuffer)
        Mem_Free(entryBuffer);

    return 1;
}

HOOK(0x006BD010, C_008605d8_ReadEnvironmentChunk, "auto");
