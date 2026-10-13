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

CSF_FUNC(int, __fastcall, CPlayerCharacter_InitStateFlag, 0x0048E270, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Walks the entry array of C_008342e8 (stride 28 bytes). For every entry whose
// flag bit 3 is set and whose signed id at +0x08 is non-negative, resolves the
// entry through the scene handle table and forwards a2 to the resolved object's
// slot 11 (+0x2C) method. Returns the last value assigned during the walk.
static int* __fastcall C_008342e8_NotifyActiveEntries(C_008342e8* self, void* /*edx*/, int a2)
{
    CPlayerCharacter_InitStateFlag(self, nullptr, a2);

    uint32_t entryCount = self->field_01a0;
    int* result = reinterpret_cast<int*>(static_cast<uintptr_t>(entryCount));
    if (entryCount != 0) {
        uint32_t entryOffset = 0;
        uint32_t remaining = entryCount;
        do {
            uint8_t* entry = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(self->field_0198) + entryOffset);
            result = reinterpret_cast<int*>(entry);

            if ((CSF_AT(uint32_t, entry, 0x18) & 8) != 0 && CSF_AT(int, entry, 0x08) >= 0) {
                int* resolved = reinterpret_cast<int*>(
                    CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                         static_cast<int>(reinterpret_cast<uintptr_t>(entry))));
                result = resolved;
                if (result) {
                    using NotifyFn = int(__fastcall*)(void*, void*, int);
                    NotifyFn notify = reinterpret_cast<NotifyFn>(CSF_AT(void**, result, 0)[11]);
                    result = reinterpret_cast<int*>(notify(result, nullptr, a2));
                }
            }

            entryOffset += 28;
            --remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x0049EC40, C_008342e8_NotifyActiveEntries, "auto");
