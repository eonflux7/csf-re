#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class constructor, called with this in ecx and a2 pushed (ret 4).
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (void* self, void* edx, int a2));

// Constructs a C_0083b2e0 task: runs the base-task constructor, clears the two
// trailing state fields, then installs this class's vtable and the secondary
// base vtable. Returns the object pointer, as the original does.
static uint32_t* __fastcall C_0083b2e0_Construct(C_0083b2e0* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(self, nullptr, a2);

    self->field_0040 = 0;
    self->field_0044 = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083B2E0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083B2C0));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004CF4F0, C_0083b2e0_Construct, "auto");
