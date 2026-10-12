#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall DevConsole_SetScaledValue(void* self, void* /*edx*/,
                                              float value, int state)
{
    const float scaledValue = value * CSF_AT(float, self, 0x14);
    CSF_AT(int, self, 0x08) = state;
    CSF_AT(float, self, 0x0C) = scaledValue;

    void* linkedObject = CSF_AT(void*, self, 0x04);
    if (linkedObject)
        CSF_AT(float, linkedObject, 0x24) = scaledValue;

    return state;
}

HOOK(0x004F9190, DevConsole_SetScaledValue, "auto");
