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

// GfxSkin_SetKeyedValue: __thiscall(void* self, int key, unsigned value) -> callee pops 8 (bound as __fastcall)
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));

// Constructor-style initializer for C_00861bd8. Takes a2 (stored as the owner pointer in field_0008)
// and a3 (when nonzero, applies three keyed skin settings to a2). Returns this.
static void* __fastcall C_00861bd8_InitWithSkinFlags(C_00861bd8* self, void* /*edx*/, int** a2, char a3)
{
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0082D428);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861BD8);
    self->field_0004 = (uint32_t)CSF_ADDR_VT(0x00861BBC);
    self->field_0008 = (uint32_t)a2;

    if (a3) {
        GfxSkin_SetKeyedValue(reinterpret_cast<void*>(self->field_0008), nullptr, 8, 1);
        GfxSkin_SetKeyedValue(reinterpret_cast<void*>(self->field_0008), nullptr, 6, 1);
        GfxSkin_SetKeyedValue(reinterpret_cast<void*>(self->field_0008), nullptr, 20, 2);
    }

    self->field_000c = 0;
    self->field_0010 = 0.0f;
    return self;
}

HOOK(0x006E5210, C_00861bd8_InitWithSkinFlags, "auto");
