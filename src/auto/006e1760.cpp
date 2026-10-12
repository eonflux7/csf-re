#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: AssignStringSubstring(this, source, position = 0, count = -1), __thiscall, ret 12.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the eight dwords at +0x1C..+0x38 from the source object into this one,
// after first assigning the string part via AssignStringSubstring. Returns this.
static C_0082d410* __fastcall C_0082d410_CopyDataFrom(C_0082d410* self, void* /*edx*/, std::uint32_t* source)
{
    AssignStringSubstring(self, nullptr, source, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);

    return self;
}

HOOK(0x006E1760, C_0082d410_CopyDataFrom, "auto");
