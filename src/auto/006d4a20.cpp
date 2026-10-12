#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x004109E0: AssignStringSubstring(this, source, position, count), 3 stack args -> ret 12
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// 0x004419F0: MissionScript_ProcessEntry, __thiscall with 1 stack arg -> ret 4
CSF_FUNC(uint32_t*, __fastcall, MissionScript_ProcessEntry_4419F0, 0x004419F0,
         (void* self, void* edx, uint32_t* a2));

// Copies the state of a2 into this (C_0082d410 init-from-source).
// Original: __thiscall, ret 4, returns this in eax.
static int __fastcall C_0082d410_InitFromSource(C_0082d410* self, void* /*edx*/, C_0082d410* a2)
{
    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint32_t, self, 0x14) = 0;
    self->field_0004 &= 0xFFFFFF00u;   // LOBYTE(this->field_0004) = 0

    AssignStringSubstring(self, nullptr, a2, 0, (std::uint32_t)-1);

    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, a2, 0x1C);

    MissionScript_ProcessEntry_4419F0(
        static_cast<void*>(reinterpret_cast<std::uint8_t*>(self) + 0x20),
        nullptr,
        reinterpret_cast<uint32_t*>(reinterpret_cast<std::uint8_t*>(a2) + 0x20));

    CSF_AT(uint32_t, self, 0x30) = CSF_AT(uint32_t, a2, 0x30);
    CSF_AT(uint32_t, self, 0x34) = CSF_AT(uint32_t, a2, 0x34);
    CSF_AT(uint32_t, self, 0x38) = CSF_AT(uint32_t, a2, 0x38);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x006D4A20, C_0082d410_InitFromSource, "auto");
