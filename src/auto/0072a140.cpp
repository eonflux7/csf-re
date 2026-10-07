#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable pointer installed into the object at offset +0x00.
CSF_GLOBAL(uint32_t, g_vtbl_C_00865758, 0x00865758);

// Initializes a C_00865758 object: installs its vtable and sets the default
// member values (0x24 = 0, 0x28 = 15, byte at 0x14 = 0, 0x30 = 0, 0x34 = 1.0f).
static C_00865758* __fastcall C_00865758_Init(C_00865758* self, void* /*edx*/)
{
    self->field_0028 = 0xF;                                     // +0x28
    self->field_0024 = 0;                                       // +0x24
    CSF_AT(uint8_t, self, 0x14) = 0;                            // +0x14 (single byte store)
    self->vtbl = reinterpret_cast<void**>(&g_vtbl_C_00865758);  // +0x00
    self->field_0030 = 0;                                       // +0x30
    self->field_0034 = 1.0f;                                    // +0x34 = 0x3F800000
    return self;
}

HOOK(0x0072A140, C_00865758_Init, "auto");
