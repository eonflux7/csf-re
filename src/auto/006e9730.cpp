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

CSF_FUNC(int, __fastcall, sub_6E9030, 0x006E9030, (void* self, void* edx, int a2));

static int __fastcall C_00861e98_SetResolvedFlagBit0(C_00861e98* self, void* /*edx*/, int a2, char a3)
{
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x1C));
    if (result)
    {
        result = sub_6E9030(self, nullptr, a2);
        void* target = reinterpret_cast<void*>(result);
        int flags = CSF_AT(int, target, 0x14);
        if (a3)
            CSF_AT(int, target, 0x14) = flags | 1;
        else
            CSF_AT(int, target, 0x14) = flags & 0xFFFFFFFE;
    }
    return result;
}

HOOK(0x006E9730, C_00861e98_SetResolvedFlagBit0, "auto");
