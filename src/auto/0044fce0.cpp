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

// Sets the scene's focus to the entry a2 (or clears focus when a2 == 0).
// Every other non-disabled entry gets SetMenuContextState(entry, 1, 1).
// The focused entry is then re-enabled via SetMenuContextState(entry, 0, 1).

CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));

static char __fastcall CScene_SetFocusEntry(C_008312b0* self, void* /*edx*/, int requestedEntry)
{
    const uint32_t entryCount = CSF_AT(uint32_t, self, 0x68);
    int focusIndex = -1;

    for (uint32_t i = 0; i < entryCount; ++i)
    {
        const uint32_t entryArray = CSF_AT(uint32_t, self, 0x60);
        const int entry = *reinterpret_cast<int*>(entryArray + 4 * i);

        if (requestedEntry == entry)
        {
            focusIndex = static_cast<int>(i);
        }
        else if (CSF_AT(uint8_t, entry, 0x228) == 0)
        {
            SetMenuContextState(reinterpret_cast<void*>(entry), nullptr, 1, 1);
        }
    }

    char result = static_cast<char>(requestedEntry);
    if (requestedEntry != 0)
    {
        if (focusIndex != -1)
        {
            CSF_AT(int, self, 0x70) = focusIndex;
            const uint32_t entryArray = CSF_AT(uint32_t, self, 0x60);
            const int focusEntry = *reinterpret_cast<int*>(entryArray + 4 * static_cast<uint32_t>(focusIndex));
            result = static_cast<char>(CSF_AT(uint8_t, focusEntry, 0x228));
            if (result)
            {
                return SetMenuContextState(reinterpret_cast<void*>(focusEntry), nullptr, 0, 1);
            }
        }
    }
    else
    {
        CSF_AT(int, self, 0x70) = -1;
    }
    return result;
}

HOOK(0x0044FCE0, CScene_SetFocusEntry, "auto");
