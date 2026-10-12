#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

static int __fastcall EntryList_GetEntryCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x04);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x08);
    const int32_t byteCount =
        static_cast<int32_t>(static_cast<uint32_t>(end) -
                             static_cast<uint32_t>(begin));
    return byteCount / 52;
}

HOOK(0x00471690, EntryList_GetEntryCount, "auto");
