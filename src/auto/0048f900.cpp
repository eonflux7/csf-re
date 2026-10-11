#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __fastcall EnableObjectAndLinkedFlag(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x171) = 1;
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(uint8_t, linkedObject, 0x9) = 1;
    return 1;
}

HOOK(0x0048F900, EnableObjectAndLinkedFlag, "auto");
