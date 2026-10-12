#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

static int __fastcall BddTables_GetEntryCount(void* self, void* /*edx*/)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const uint32_t end = CSF_AT(uint32_t, self, 0x08);
    const int32_t byteCount = static_cast<int32_t>(end - begin);
    return byteCount / 552;
}

HOOK(0x0051AD70, BddTables_GetEntryCount, "auto");
