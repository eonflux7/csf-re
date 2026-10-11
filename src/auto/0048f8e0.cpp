#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __fastcall EnableObjectAndLinkedFlagAt19(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x171) = 1;
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint8_t, linkedObject, 0x13) = 1;
    return 1;
}

HOOK(0x0048F8E0, EnableObjectAndLinkedFlagAt19, "auto");
