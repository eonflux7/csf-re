#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833708_FindEntry(C_00833708* self, void* /*edx*/, int key)
{
    int index = self->vSlotC0(key);
    if (index != -1)
        return reinterpret_cast<int>(
            self->vSlotB4(reinterpret_cast<void*>(index)));
    return index;
}

HOOK(0x00489B80, C_00833708_FindEntry, "auto");
