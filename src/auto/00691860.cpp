#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// The callee takes `this` in ecx (the original sets `mov ecx, ebx` before the call) and pops
// its 3 stack arguments (ret 12), so it is a __thiscall, bound as __fastcall with edx unused.
CSF_FUNC(char*, __fastcall, FindKeyInBlockRange, 0x00691620,
         (S_00691860* self, void* edx, char* a1, char* a2, unsigned int a3));

// Walks the slot table of S_00691860 and returns the element whose first dword equals `key`.
// Returns nullptr when no slot matches (or when the matched element is the one-past-end slot).
static char* __fastcall S_00691860_FindKeyInSlotTable(S_00691860* self, void* /*edx*/, unsigned int a2)
{
    uint32_t slotIndex = 0;

    if (self->field_000c != 0)
    {
        while (true)
        {
            uint32_t* slotTable = reinterpret_cast<uint32_t*>(self->field_0004);
            uint8_t* entry = reinterpret_cast<uint8_t*>(slotTable[slotIndex]);

            if (CSF_AT(uint32_t, entry, 8) == 0)
                break;

            uint32_t blockBase = CSF_AT(uint32_t, entry, 0);
            uint32_t lastSlotAddr = 16u * CSF_AT(uint32_t, entry, 8) + blockBase - 16u;
            char* lastSlot = reinterpret_cast<char*>(lastSlotAddr);

            char* result = FindKeyInBlockRange(self, nullptr,
                                               reinterpret_cast<char*>(blockBase),
                                               lastSlot, a2);

            if (reinterpret_cast<uintptr_t>(result) <= reinterpret_cast<uintptr_t>(lastSlot)
                && CSF_AT(uint32_t, result, 0) == a2)
            {
                return result;
            }

            uint32_t slotCount = self->field_000c;
            if (slotIndex == slotCount - 1)
                return (result == lastSlot + 16) ? nullptr : result;

            ++slotIndex;
            if (slotIndex >= slotCount)
                return nullptr;
        }
    }

    return nullptr;
}

HOOK(0x00691860, S_00691860_FindKeyInSlotTable, "auto");
