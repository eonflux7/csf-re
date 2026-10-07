#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets the byte at offset 0x17E of the object to 0.
static void __fastcall ClearFlag17E(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x17E) = 0;
}

HOOK(0x536640, ClearFlag17E, "auto");
