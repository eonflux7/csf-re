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

CSF_FUNC(int, __fastcall, sub_46F390, 0x0046F390, (void* self, void* edx, int a2));

// Advances the cursor stored at self+0x1c to the next entry of the table at
// owner(self+0x10)->table(+0x50). Each table entry is 8 bytes: a "next index"
// dword followed by a dword that is used when the next index is -1.
// Returns the low byte of the selected value (char).
static char __fastcall Cursor_AdvanceToNextEntry(void* self, void* /*edx*/, int a2, int a3, void* a4)
{
    (void)a2;
    (void)a3;

    // *(*(a4) + 4)[1]: flag byte taken through the two-level pointer chain.
    uint8_t* firstLevel = *reinterpret_cast<uint8_t**>(a4);
    uint8_t* secondLevel = *reinterpret_cast<uint8_t**>(firstLevel + 4);
    uint8_t flag = secondLevel[1];

    uint32_t v5 = flag;
    if (flag == 0)
    {
        uint32_t cursor = CSF_AT(uint32_t, self, 0x1C);
        void* owner = CSF_AT(void*, self, 0x10);
        int index = sub_46F390(owner, nullptr, static_cast<int>(cursor));

        // ecx is reloaded from self+0x10 after the call.
        uint8_t* ownerAfterCall = reinterpret_cast<uint8_t*>(CSF_AT(void*, self, 0x10));
        uint32_t* table = *reinterpret_cast<uint32_t**>(ownerAfterCall + 0x50);
        uint32_t* entry = table + 2 * index;

        uint32_t next = entry[0];
        v5 = reinterpret_cast<uintptr_t>(entry);
        if (next == 0xFFFFFFFFu)
        {
            v5 = entry[1];
            CSF_AT(uint32_t, self, 0x1C) = v5;
        }
        else
        {
            CSF_AT(uint32_t, self, 0x1C) = next;
        }
    }
    return static_cast<char>(v5);
}

HOOK(0x00463580, Cursor_AdvanceToNextEntry, "auto");
