#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_LookupByOffsetFromField0C(C_00834fa8* self, void* /*edx*/)
{
    int offsetBase = CSF_AT(int, self, 0x0C);
    return CharTask_LookupByOffset(self, nullptr, offsetBase + 100);
}

HOOK(0x004D4B00, C_00834fa8_LookupByOffsetFromField0C, "auto");
