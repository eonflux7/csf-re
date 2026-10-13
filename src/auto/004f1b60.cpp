#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class constructor step (bound by address; the original passes its `this` and a2 through).
CSF_FUNC(uint32_t*, __fastcall, C_0083ef30_ConstructBase, 0x004F0E10, (void* self, void* edx, int a2));

// Constructor for C_0083f040 (derived from C_0083ef30).
// Runs the base constructor, installs this class's primary vtable, stores a secondary
// vtable at +4, clears the float at +0x44, and returns the object pointer.
static uint32_t* __fastcall C_0083f040_Construct(C_0083f040* self, void* /*edx*/, int a2)
{
    C_0083ef30_ConstructBase(self, nullptr, a2);
    self->vtbl = CSF_VTBL(0x0083F040);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083F024));
    self->field_0044 = 0.0f;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004F1B60, C_0083f040_Construct, "auto");
