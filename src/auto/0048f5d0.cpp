#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall EnableAndSetValue(void* self, void* /*edx*/, uint32_t* value)
{
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint8_t, self, 0x171) = 1;
    CSF_AT(uint8_t, linkedObject, 0x7) = 1;
    CSF_AT(uint32_t, self, 0xDC) = *value;
    return value;
}

HOOK(0x0048F5D0, EnableAndSetValue, "auto");
