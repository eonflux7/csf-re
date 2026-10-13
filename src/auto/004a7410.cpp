#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Looks up a task by the offset stored at +0x0C, shifted by 0x40.
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_LookupTaskAtStoredOffset(C_00834fa8* self, void* /*edx*/)
{
    int storedOffset = CSF_AT(int, self, 0x0C);
    return CharTask_LookupByOffset(self, nullptr, storedOffset + 0x40);
}

HOOK(0x004A7410, C_00834fa8_LookupTaskAtStoredOffset, "auto");
