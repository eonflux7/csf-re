#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall LevelEntry_Destroy(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0xD8) >= 0x10u) {
        void* block = CSF_AT(void*, self, 0xC4);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0xD8) = 15;
    CSF_AT(uint32_t, self, 0xD4) = 0;
    CSF_AT(uint8_t, self, 0xC4) = 0;

    if (CSF_AT(uint32_t, self, 0xB8) >= 0x10u) {
        void* block = CSF_AT(void*, self, 0xA4);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(uint32_t, self, 0xB8) = 15;
    CSF_AT(uint32_t, self, 0xB4) = 0;
    CSF_AT(uint8_t, self, 0xA4) = 0;

    void* block = CSF_AT(void*, self, 0x94);
    if (block)
        Mem_FreeDebug(block);
    CSF_AT(uint32_t, self, 0x94) = 0;
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;

    int result = CSF_AT(int, self, 0x84);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    CSF_AT(uint32_t, self, 0x84) = 0;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;

    if (CSF_AT(uint32_t, self, 0x20) >= 0x10u) {
        result = CSF_AT(int, self, 0x0C);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 15;
    CSF_AT(uint8_t, self, 0x0C) = 0;

    return result;
}

HOOK(0x00775090, LevelEntry_Destroy, "auto");
