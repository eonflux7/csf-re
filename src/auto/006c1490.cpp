#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor of C_00860178 (bind as per packet).
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void* edx));

// Constructs a C_008607f8 shape allocator: runs the base constructor, clears the two
// bookkeeping fields, installs this class's vtable, and returns the object pointer.
static uint32_t* __fastcall ShapeAllocatorBase_Construct(C_008607f8* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->vtbl = CSF_VTBL(0x008607F8);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006C1490, ShapeAllocatorBase_Construct, "auto");
