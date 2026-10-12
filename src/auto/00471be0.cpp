#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __fastcall AdvanceEntryCursor(void* self, void* /*edx*/)
{
    const uint32_t nextIndex = CSF_AT(uint32_t, self, 0x08) + 1;
    CSF_AT(uint32_t, self, 0x08) = nextIndex;

    const uint32_t begin = CSF_AT(uint32_t, self, 0x10);
    if (begin)
    {
        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x14) - begin);
        if (nextIndex < static_cast<uint32_t>(byteCount >> 2))
            return 1;
    }

    const uint32_t currentBegin = CSF_AT(uint32_t, self, 0x10);
    if (currentBegin)
    {
        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x14) - currentBegin);
        CSF_AT(uint32_t, self, 0x08) =
            static_cast<uint32_t>(byteCount >> 2) - 1;
    }
    else
    {
        CSF_AT(uint32_t, self, 0x08) = UINT32_MAX;
    }
    return 0;
}

HOOK(0x00471BE0, AdvanceEntryCursor, "auto");
