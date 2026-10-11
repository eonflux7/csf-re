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

// Original 0x0064B250 (__fastcall-style callee, ecx = this, edx unused)
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

// Increments one of three 16-bit menu stat counters depending on the selection
// index passed in. Returns the selection-derived value as the original does.
static char __fastcall CMenuMpStats_IncrementStatCounter(CMenuMpStats* self, void* /*edx*/, char selection)
{
    char result = (char)Sub_64B250(self, nullptr);
    if (result)
    {
        result = selection;
        if (selection)
        {
            result = selection - 1;
            if (selection == 1)
            {
                ++CSF_AT(uint16_t, self, 0x680);
            }
            else
            {
                result = selection - 2;
                if (selection == 2)
                    ++CSF_AT(uint16_t, self, 0x69E);
            }
        }
        else
        {
            ++CSF_AT(uint16_t, self, 0x68C);
        }
    }
    return result;
}

HOOK(0x740D50, CMenuMpStats_IncrementStatCounter, "auto");
