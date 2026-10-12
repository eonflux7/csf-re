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

// Allocator for the 0x38-byte, 64-aligned child buffer (cdecl, 4 stack args).
CSF_FUNC(int, __cdecl, AllocateAlignedBuffer, 0x00690EB0, (std::size_t size, int alignment, int option1, int option2));

// Initializes a C_008672e4 child object: sets the vtable, zeroes state,
// stores the parent/owner value, allocates a 0x38-byte aligned buffer and
// sets the default color/alpha values (255.0). Returns the object.
static C_008672e4* __fastcall C_008672e4_InitChildObject34(C_008672e4* self, void* /*edx*/, int a2)
{
    self->vtbl = CSF_VTBL(0x008672E4);
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0.0f;
    self->field_0030 = 1.0f;
    self->field_0004 = static_cast<uint32_t>(a2);
    self->field_0008 = 0;
    self->field_000c = static_cast<uint32_t>(AllocateAlignedBuffer(0x38, 64, 0, 0));
    self->field_0024 = 255.0f;
    self->field_0028 = 255.0f;
    self->field_002c = 255.0f;
    self->field_0010 = 1;
    self->field_0030 = 255.0f;
    return self;
}

HOOK(0x738130, C_008672e4_InitChildObject34, "auto");
