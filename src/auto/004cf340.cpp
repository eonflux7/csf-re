#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004D1660 AiAutonomous_ConstructTaskBase, __thiscall, one stack arg, ret 4.
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (void* self, void* edx, int a2));

// Constructs C_0083b1f8 (task derived from C_0083b958 / TCharTask) in place.
// Original: __thiscall, one stack argument, ret 4. Returns the object pointer.
static uint32_t* __fastcall C_0083b1f8_Construct(C_0083b1f8* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(self, nullptr, a2);

    self->field_0040 = 0;
    self->field_0048 = 0;
    self->vtbl = CSF_VTBL(0x0083B1F8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083B1DC));
    self->field_0044 = 1;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004CF340, C_0083b1f8_Construct, "auto");
