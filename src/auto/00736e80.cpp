#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, UpdateComputedValue, 0x00736D50, (void* self, void* edx));

static int __fastcall C_008670f0_GetSecondaryComputedValue(C_008670f0* self, void* /*edx*/)
{
    const uint32_t savedValue = self->field_00bc;
    self->field_0010 = 1;
    self->field_00bc = savedValue ? savedValue : 1;
    UpdateComputedValue(self, nullptr);
    const int result = static_cast<int>(self->field_00d4);
    self->field_00bc = savedValue;
    return result;
}

HOOK(0x00736E80, C_008670f0_GetSecondaryComputedValue, "auto");
