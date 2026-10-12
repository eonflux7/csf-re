#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

static S_004f21d0* __fastcall S_004f21d0_AssignString(
    S_004f21d0* self, void* /*edx*/, void* source)
{
    AssignStringRange_488830(self, nullptr, source, 0, 0xFFFFFFFF);
    return self;
}

HOOK(0x004F25F0, S_004f21d0_AssignString, "auto");
