#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer used for TAutonomo and each embedded sub-object.
CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

// Constructor for TAutonomo: initializes the CharTask base, the embedded
// sub-objects at +0x14, +0x28, +0x3C and +0x50, then zeroes the state fields.
static uint32_t* __fastcall TAutonomo_Construct(TAutonomo* self, void* /*edx*/) {
    CharTask_InitBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x0083DA30);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083DA10);

    // Sub-object at +0x14
    void* sub14 = (uint8_t*)self + 0x14;
    CharTask_InitBase(sub14, nullptr);
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x0083D460);
    CSF_AT(uint32_t, self, 0x18) = (uint32_t)CSF_ADDR_DATA(0x0083D440);

    // Sub-object at +0x28
    void* sub28 = (uint8_t*)self + 0x28;
    CharTask_InitBase(sub28, nullptr);
    CSF_AT(void*, self, 0x28) = CSF_VTBL(0x0083D520);
    CSF_AT(uint32_t, self, 0x2C) = (uint32_t)CSF_ADDR_DATA(0x0083D504);

    // Sub-object at +0x3C
    void* sub3C = (uint8_t*)self + 0x3C;
    CharTask_InitBase(sub3C, nullptr);
    CSF_AT(void*, self, 0x3C) = CSF_VTBL(0x0083D5D8);
    CSF_AT(uint32_t, self, 0x40) = (uint32_t)CSF_ADDR_DATA(0x0083D5B8);

    // Sub-object at +0x50
    void* sub50 = (uint8_t*)self + 0x50;
    CharTask_InitBase(sub50, nullptr);
    CSF_AT(void*, self, 0x50) = CSF_VTBL(0x0083D690);
    CSF_AT(uint32_t, self, 0x54) = (uint32_t)CSF_ADDR_DATA(0x0083D670);

    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4D7870, TAutonomo_Construct, "auto");
