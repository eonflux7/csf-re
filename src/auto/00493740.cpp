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

CSF_FUNC(std::uint32_t*, __fastcall, C_00833908_Construct, 0x00490940, (C_00833908* self, void* , int objectId, int stateCode));

// Constructor for C_00833cf8: runs the base constructor, zeroes the trailing
// counters, installs the vtable chain, and sets the object id to -1.
static std::uint32_t* __fastcall C_00833cf8_Construct(C_00833cf8* self, void* /*edx*/, int a2, int a3)
{
    C_00833908_Construct(self, nullptr, a2, a3);

    self->field_0188 = 0;
    self->field_018c = 0;
    self->field_0190 = 0;

    self->vtbl = CSF_VTBL(0x00833CF8);
    self->field_0004 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(CSF_VTBL(0x00833C78)));
    CSF_AT(std::uint32_t, self, 0x008) = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(CSF_VTBL(0x00833C74)));
    self->field_000c = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(CSF_VTBL(0x00833C68)));
    CSF_AT(std::uint32_t, self, 0x010) = static_cast<std::uint32_t>(CSF_ADDR_DATA(0x00833C60));
    self->field_0014 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(CSF_VTBL(0x00833C5C)));

    self->field_0184 = 0xFFFFFFFFu;

    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x00493740, C_00833cf8_Construct, "auto");
