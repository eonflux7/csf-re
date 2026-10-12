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

// Base-class constructor (C_0082e598), __thiscall with one stack argument.
CSF_FUNC(uint32_t*, __fastcall, C_0082e598_Construct, 0x0041E1D0, (C_0082e598* self, void* /*edx*/, int initialValue));

// Initializes the embedded C_0082fe88 sub-object at this+0x24.
CSF_FUNC(uint32_t*, __fastcall, InitEmbeddedFe88Subobject, 0x004262C0, (void* self, void* /*edx*/));

// Constructor for C_0082faf8. Original: __thiscall, one stack argument, `ret 4`.
static int __fastcall C_0082faf8_Construct(C_0082faf8* self, void* /*edx*/, int sourceObject)
{
    C_0082e598_Construct(self, nullptr, sourceObject);

    // Vtable for C_0082faf8 and its secondary-base vtable.
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x0082FAF8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082FADC));

    InitEmbeddedFe88Subobject(reinterpret_cast<uint8_t*>(self) + 0x24, nullptr);

    self->field_0020 = 0;
    self->field_0050 = 0.0f;

    // Value read from the source object at +0x184 after the sub-object init.
    float sourceSpeed = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(sourceObject)), 0x184);

    self->field_0058 = 0.0f;
    self->field_005c = 0.0f;
    self->field_0074 = 0;
    self->field_0060 = 0;
    self->field_0064 = 0xFFFFFFFFu;
    self->field_0068 = 0xFFFFFFFFu;
    self->field_006c = 0xFFFFFFFFu;
    self->field_0054 = sourceSpeed;

    return reinterpret_cast<int>(self);
}

HOOK(0x423FD0, C_0082faf8_Construct, "auto");
