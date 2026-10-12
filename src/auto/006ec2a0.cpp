#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall GfxCollision_Reset(void* self, void* /*edx*/)
{
    void* block = CSF_AT(void*, self, 0x04);
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;

    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x04) = nullptr;
    }

    CSF_AT(uint8_t, self, 0x19) = 0;
    CSF_AT(uint8_t, self, 0x18) = 1;
    CSF_AT(uint8_t, self, 0x38) = 1;
    return 1;
}

HOOK(0x006EC2A0, GfxCollision_Reset, "auto");
