#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address, signatures copied from the packet.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_74CC00, 0x0074CC00, (void* self, void* edx, char a2, char a3));

// Applies a signed delta to one of the two multiplayer stat counters (offset 0x44 when a2 == 1,
// offset 0x50 otherwise). The counter is only written when the value actually changes.
// When the menu is not in the stats state, the delta is forwarded to sub_74CC00 instead.
static int __fastcall CMenuMpStats_ApplyStatDelta(CMenuMpStats* self, void* /*edx*/, char a2, char a3)
{
    if (Sub_64B250(self, nullptr))
    {
        if (a2 == 1)
        {
            int oldValue = CSF_AT(int, self, 0x44);
            int result = oldValue + static_cast<int>(a3);
            if (result != oldValue)
                CSF_AT(int, self, 0x44) = result;
            return result;
        }
        else
        {
            int oldValue = CSF_AT(int, self, 0x50);
            int result = oldValue + static_cast<int>(a3);
            if (result != oldValue)
                CSF_AT(int, self, 0x50) = result;
            return result;
        }
    }
    else
    {
        return sub_74CC00(self, nullptr, a2, a3);
    }
}

HOOK(0x00747FB0, CMenuMpStats_ApplyStatDelta, "auto");
