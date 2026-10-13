#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);

// Scans a 100-byte-entry table whose range is [field_0010, field_0014) and returns
// the index of the first entry whose +0x14 matches a2->+4 and whose +0x1C matches
// the current scene index. Returns -1 when no entry matches.
static int __fastcall S_0044bc20_FindMatchingEntryIndex(S_0044bc20* self, void* /*edx*/, int a2)
{
    int foundIndex = -1;

    uint32_t entriesBegin = self->field_0010;
    uint32_t entryCount = 0;
    if (entriesBegin != 0)
    {
        // Signed division of the byte span by 100 via multiply-high:
        // 0x51EB851F is the reciprocal magic for /100; the high dword is shifted by 5
        // and corrected by its sign bit (truncation toward zero for negative spans).
        int byteSpan = (int)(self->field_0014 - entriesBegin);
        int64_t product = (int64_t)byteSpan * (int64_t)0x51EB851F;
        int32_t highDword = (int32_t)(product >> 32);
        int32_t quotient = (highDword >> 5) + (int32_t)(((uint32_t)highDword) >> 31);
        entryCount = (uint32_t)quotient;
    }

    uint32_t entryIndex = 0;
    uint32_t byteOffset = 0;
    while (entryIndex < entryCount)
    {
        uint32_t currentBegin = self->field_0010;
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(currentBegin + byteOffset));

        int entrySourceId = CSF_AT(int, entry, 0x14);
        int wantedSourceId = CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), 4);

        if (entrySourceId == wantedSourceId && CSF_AT(int, entry, 0x1C) == g_CurrentSceneIndex)
        {
            foundIndex = (int)entryIndex;
        }

        ++entryIndex;
        byteOffset += 100;
        if (foundIndex != -1)
            break;
    }

    return foundIndex;
}

HOOK(0x0044BC70, S_0044bc20_FindMatchingEntryIndex, "auto");
