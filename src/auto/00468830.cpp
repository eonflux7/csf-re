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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall MissionScript_ResetPendingBlock(void* self, void* /*edx*/, char flag)
{
    CSF_AT(uint8_t, self, 0xAC) = static_cast<uint8_t>(flag);

    int result = CSF_AT(int, self, 0xB0);
    CSF_AT(int, self, 0xB4) = 0;
    CSF_AT(int, self, 0xB8) = 0;
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(result));
        CSF_AT(int, self, 0xB0) = 0;
    }
    return result;
}

HOOK(0x00468830, MissionScript_ResetPendingBlock, "auto");
