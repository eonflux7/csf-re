#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall GenericObject_SetStoredValue0194(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x194) = value;
    return value;
}

HOOK(0x49EC00, GenericObject_SetStoredValue0194, "auto");
