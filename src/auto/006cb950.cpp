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

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

// Scans the entry list stored in the 16-byte slot at this + 16*slotIndex and returns the
// index of the first entry whose name (returned by its object's name getter) matches
// `name` case-insensitively, or -1 if none matches.
static int __fastcall C_00860bb8_FindSlotEntryIndexByName(C_00860bb8* self, void* /*edx*/, int a2, char* String2)
{
    uint8_t* slot = reinterpret_cast<uint8_t*>(self) + 16 * a2;
    int foundIndex = -1;
    uint32_t i = 0;
    uint32_t entryCount = CSF_AT(uint32_t, slot, 0x2c);

    if (entryCount != 0) {
        do {
            if (foundIndex != -1) {
                break;
            }

            // Re-read the entry table pointer each iteration, as the original does.
            auto* entryTable = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, slot, 0x24)));
            void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(entryTable[i]));

            // entry->vtbl[0x80/4](entry, 0) -> nameProvider
            void* nameProvider = ((void* (__fastcall*)(void*, void*, int))(CSF_AT(void**, entry, 0)[0x80 / 4]))(entry, nullptr, 0);

            // nameProvider->vtbl[0x14/4]() -> const char* name
            const char* name = ((const char* (__fastcall*)(void*, void*))(CSF_AT(void**, nameProvider, 0)[0x14 / 4]))(nameProvider, nullptr);

            if (name && String2 && __stricmp(name, String2) == 0) {
                foundIndex = static_cast<int>(i);
            }

            ++i;
        } while (i < entryCount);
    }

    return foundIndex;
}

HOOK(0x006CB950, C_00860bb8_FindSlotEntryIndexByName, "auto");
