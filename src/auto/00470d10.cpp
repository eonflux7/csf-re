#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, MissionScript_ClearBitRange, 0x00470440,
         (void* self, void* edx, int* a2, unsigned int a3, int* a4,
          unsigned int a5, uint32_t* a6));
CSF_FUNC(int, __cdecl, MissionScript_FreeEntry, 0x004714E0,
         (void* Block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall MissionScript_ClearOwnedEntries(void* self, void* /*edx*/)
{
    (void)CSF_VTBL(0x00470440);

    const uint32_t begin = CSF_AT(uint32_t, self, 0x04);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, self, 0x08) - begin) >> 2)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        if ((CSF_AT(uint32_t*, self, 0x18)[index >> 5] &
             (uint32_t{1} << (index & 0x1F))) == 0)
            continue;

        S_0069b1c0* entry = CSF_AT(S_0069b1c0**, self, 0x04)[index];
        if (!entry)
            continue;

        S_0069b1c0* child =
            reinterpret_cast<S_0069b1c0*>(entry->field_0004);
        if (child)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, child, 0)[0])(child, nullptr, 0);
            MissionScript_FreeEntry(child);
        }

        entry->field_0004 = 0;
        MissionScript_FreeEntry(entry);
        CSF_AT(S_0069b1c0**, self, 0x04)[index] = nullptr;
    }

    const uint32_t bitCount = CSF_AT(uint32_t, self, 0x10);
    uint32_t* bits = CSF_AT(uint32_t*, self, 0x18);
    uint32_t* endWord = bits;
    uint32_t endBit = 0;
    if (bitCount)
    {
        endWord += bitCount >> 5;
        endBit = bitCount & 0x1F;
    }

    int rangeResult[0xC / sizeof(int)] = {};
    MissionScript_ClearBitRange(
        static_cast<uint8_t*>(self) + 0x10, nullptr, rangeResult, 0,
        reinterpret_cast<int*>(bits), endBit, endWord);

    int result = 0;
    void* storage = CSF_AT(void*, self, 0x04);
    if (storage)
        result = Mem_FreeDebug(storage);

    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    return result;
}

HOOK(0x00470D10, MissionScript_ClearOwnedEntries, "auto");
