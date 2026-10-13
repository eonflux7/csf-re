#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CInputDirectInput_StoreDeviceIndex0C94(CInputDirectInput* self, void* /*edx*/, int value)
{
    self->field_0c94 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x713270, CInputDirectInput_StoreDeviceIndex0C94, "auto");
