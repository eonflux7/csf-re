#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall C_008670f0_SetFourWordValue(
    C_008670f0* self, void* /*edx*/, uint32_t* value)
{
    uint32_t* result = &self->field_0098;
    if (self->field_0098 != value[0] ||
        CSF_AT(uint32_t, self, 0x9C) != value[1] ||
        CSF_AT(uint32_t, self, 0xA0) != value[2] ||
        CSF_AT(uint32_t, self, 0xA4) != value[3])
    {
        self->field_0010 = 1;
        self->field_00d8 = 1;
        self->field_0098 = value[0];
        CSF_AT(uint32_t, self, 0x9C) = value[1];
        CSF_AT(uint32_t, self, 0xA0) = value[2];
        CSF_AT(uint32_t, self, 0xA4) = value[3];
    }
    return result;
}

HOOK(0x00732E30, C_008670f0_SetFourWordValue, "auto");
