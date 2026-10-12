#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall S_00749d70_FindObjectIndex(
    S_00749d70* self, void* /*edx*/, int objectId)
{
    for (int index = 0; index < 16; ++index)
    {
        if (CSF_AT(std::int32_t, self, 0x4C + index * 4) == objectId)
            return index;
    }

    return -1;
}

HOOK(0x00749D70, S_00749d70_FindObjectIndex, "auto");
