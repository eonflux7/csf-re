#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// __cdecl _stricmp, bound by address.
CSF_FUNC(int, __cdecl, MenuManager_StriCmp, 0x007B0C41, (const char* left, const char* right));

// Looks up a page name in a menu page table (vector-like, 28-byte entries).
// Each entry holds a std::string-like name at +4 (inline when capacity at +24 < 16,
// otherwise a pointer at +4). Returns 1 if any entry matches case-insensitively, else 0.
static int __cdecl MenuManager_ContainsPageName(char* searchName, int pageTable)
{
    void* table = reinterpret_cast<void*>(pageTable);

    uint32_t entryCount = 0;
    uint32_t beginAddr = CSF_AT(uint32_t, table, 0x04);
    if (beginAddr != 0)
    {
        // Signed division by 28 (matches the original magic-number sequence).
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, table, 0x08) - beginAddr);
        int32_t signedCount = byteSpan / 28;
        entryCount = static_cast<uint32_t>(signedCount);
    }

    uint32_t matchIndex = 0;
    if (entryCount != 0)
    {
        uint32_t byteOffset = 0;
        do
        {
            uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, table, 0x04)) + byteOffset;
            uint32_t capacity = *reinterpret_cast<uint32_t*>(entry + 0x18);

            const char* entryName;
            if (capacity < 0x10)
                entryName = reinterpret_cast<const char*>(entry + 4);
            else
                entryName = *reinterpret_cast<const char* const*>(entry + 4);

            if (MenuManager_StriCmp(entryName, searchName) == 0)
                break;

            ++matchIndex;
            byteOffset += 28;
        } while (matchIndex < entryCount);
    }

    return (matchIndex < entryCount) ? 1 : 0;
}

HOOK(0x0050C0A0, MenuManager_ContainsPageName, "auto");
