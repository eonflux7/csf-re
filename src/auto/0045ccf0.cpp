#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FindActiveEntryIndexByKey, 0x004DC5B0, (void* self, void* edx, void* keyObject));
CSF_FUNC(int, __fastcall, GetActiveEntryByIndex, 0x004DC6E0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, TryLocatePointInGrid, 0x0075CE00, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetGridEntryByIndex, 0x0075CF00, (void* self, void* edx, int a2));

// Looks up the active entry matching the key stored behind keySource, locates the
// point of that entry inside the grid, and writes the resulting grid value into
// the record referenced by resultHolder+4. Returns the low byte of the last
// computed value (matches the original's eax handling).
static int __stdcall LookupActiveEntryIntoRecord(uint32_t resultHolder, int /*unusedArg*/, uint32_t keySource)
{
    // Table pointer stored at +0x1FD8 of the global block.
    uint32_t globalBase = static_cast<uint32_t>(GetValue8B0554());
    C_0083e0d8* table = *reinterpret_cast<C_0083e0d8**>(globalBase + 0x1FD8);

    // keyObject = *(*(keySource) + 4) + 4
    uint32_t keyRecord = *reinterpret_cast<uint32_t*>(keySource);
    void* keyObject = reinterpret_cast<void*>(*reinterpret_cast<uint32_t*>(keyRecord + 4) + 4);

    // The active-entry list is the sub-object at table+0x34.
    void* activeList = reinterpret_cast<uint8_t*>(table) + 0x34;

    int entryIndex = FindActiveEntryIndexByKey(activeList, nullptr, keyObject);
    if (entryIndex == -1)
    {
        uint32_t targetRecord = *reinterpret_cast<uint32_t*>(resultHolder + 4);
        *reinterpret_cast<uint32_t*>(targetRecord + 4) = 0xFFFFFFFFu;
        return static_cast<int>(static_cast<uint8_t>(resultHolder));
    }

    int entryAddr = GetActiveEntryByIndex(activeList, nullptr, entryIndex);

    uint32_t searchResult[2] = { 0xFFFFFFFFu, 0xFFFFFFFFu };
    char found = TryLocatePointInGrid(
        table,
        nullptr,
        reinterpret_cast<float*>(static_cast<uint32_t>(entryAddr) + 16),
        searchResult);

    if (found != 0)
    {
        uint32_t outRecord = *reinterpret_cast<uint32_t*>(resultHolder + 4);
        int gridEntry = GetGridEntryByIndex(table, nullptr, static_cast<int>(searchResult[0]));
        *reinterpret_cast<uint32_t*>(outRecord + 4) = *reinterpret_cast<uint32_t*>(gridEntry);
        return static_cast<int>(static_cast<uint8_t>(gridEntry));
    }

    return static_cast<int>(static_cast<uint8_t>(found));
}

HOOK(0x0045CCF0, LookupActiveEntryIntoRecord, "auto");
