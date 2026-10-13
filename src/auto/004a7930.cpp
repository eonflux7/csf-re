#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_LookupTaskAtFieldPlus9C(C_00834fa8* self, void* /*edx*/) {
    int taskOffset = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0x9C);
    return CharTask_LookupByOffset(self, nullptr, taskOffset);
}

HOOK(0x004A7930, C_00834fa8_LookupTaskAtFieldPlus9C, "auto");
