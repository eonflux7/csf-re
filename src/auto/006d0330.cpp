#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Mem_Free / Mem_FreeDebug are __cdecl; the original pops nothing for them.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees the two growable buffers of the shape and resets both containers to
// empty small-buffer state (capacity 15, size 0). Callee pops 0 (__thiscall, ret 0).
static int __fastcall CollisionShape_FreeBuffersAndReset(void* self, void* /*edx*/)
{
    int result = 0;

    if (CSF_AT(uint32_t, self, 0x4C) != 0)
    {
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x4C)));
        CSF_AT(uint32_t, self, 0x4C) = 0;
    }

    result = static_cast<int>(CSF_AT(uint32_t, self, 0x3C));
    if (result != 0)
    {
        Mem_Free(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x3C)));
        CSF_AT(uint32_t, self, 0x3C) = 0;
    }

    if (CSF_AT(uint32_t, self, 0x34) >= 0x10)
    {
        result = static_cast<int>(CSF_AT(uint32_t, self, 0x20));
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x20)));
    }

    CSF_AT(uint32_t, self, 0x34) = 15;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint8_t, self, 0x20) = 0;

    if (CSF_AT(uint32_t, self, 0x18) >= 0x10)
    {
        result = static_cast<int>(CSF_AT(uint32_t, self, 0x04));
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x04)));
    }

    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint8_t, self, 0x04) = 0;

    return result;
}

HOOK(0x006D0330, CollisionShape_FreeBuffersAndReset, "auto");
