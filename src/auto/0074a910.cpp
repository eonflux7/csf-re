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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

CSF_GLOBAL(int, g_Value_8BC170, 0x008BC170);

static bool __cdecl NetSession_InitializeRuntime()
{
    if (g_Value_8BC170 != 0)
    {
        CoreMemory_Free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(g_Value_8BC170)));
        g_Value_8BC170 = 0;
    }

    int allocatedBlock = CoreMemory_AllocUntagged(1);
    if (allocatedBlock != 0)
    {
        uint8_t* flagByte = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(allocatedBlock));
        *flagByte = 0;
        g_Value_8BC170 = allocatedBlock;
        return flagByte != nullptr;
    }

    g_Value_8BC170 = 0;
    return false;
}

HOOK(0x0074A910, NetSession_InitializeRuntime, "auto");
