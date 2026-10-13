#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static C_0082d410* __fastcall C_0082d410_AssignStringFromSource(
    C_0082d410* self, void* /*edx*/, const void* source)
{
    AssignStringSubstring(self, nullptr, source, 0, 0xFFFFFFFF);
    return self;
}

HOOK(0x0069D8E0, C_0082d410_AssignStringFromSource, "auto");
