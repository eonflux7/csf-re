#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (C_0083b958* self, void* , int a2));

// Constructor for C_00835140 (task derived from C_0083b958 / TCharTask).
// Runs the base constructor, installs this class's primary vtable and a secondary
// vtable at +4, clears the 0x40 field, and returns the object pointer.
static uint32_t* __fastcall C_00835140_Construct(C_00835140* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(reinterpret_cast<C_0083b958*>(self), nullptr, a2);
    self->vtbl = CSF_VTBL(0x00835140);
    CSF_AT(void**, self, 0x4) = CSF_VTBL(0x00835124);
    self->field_0040 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004A8BC0, C_00835140_Construct, "auto");
