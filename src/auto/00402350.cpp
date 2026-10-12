#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_AxisX,   0x8A7D6C);
CSF_GLOBAL(float, g_AxisX_Y, 0x8A7D70);
CSF_GLOBAL(float, g_AxisX_Z, 0x8A7D74);
CSF_GLOBAL(float, g_AxisY,   0x8A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x8A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x8A7D80);
CSF_GLOBAL(float, g_AxisZ,   0x8A7D84);
CSF_GLOBAL(float, g_AxisZ_Y, 0x8A7D88);
CSF_GLOBAL(float, g_AxisZ_Z, 0x8A7D8C);

// Initializes a transform-like struct: position zeroed, half-extents 0.5, identity-ish axes from globals.
static uint32_t* __fastcall S00402350_InitTransform(S_00402350* self, void* /*edx*/)
{
    uint32_t* result = &self->field_0000;

    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;

    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;

    self->field_000c = 0x3F000000u;
    self->field_0010 = 0x3F000000u;
    self->field_0014 = 0x3F000000u;

    CSF_AT(float, self, 0x18) = g_AxisX;
    CSF_AT(float, self, 0x1C) = g_AxisX_Y;
    CSF_AT(float, self, 0x20) = g_AxisX_Z;
    CSF_AT(float, self, 0x24) = g_AxisY;
    CSF_AT(float, self, 0x28) = g_AxisY_Y;
    CSF_AT(float, self, 0x2C) = g_AxisY_Z;
    CSF_AT(float, self, 0x30) = g_AxisZ;
    CSF_AT(float, self, 0x34) = g_AxisZ_Y;
    CSF_AT(float, self, 0x38) = g_AxisZ_Z;

    return result;
}

HOOK(0x00402350, S00402350_InitTransform, "auto");
