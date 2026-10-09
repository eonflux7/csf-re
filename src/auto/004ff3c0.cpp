#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static std::uint32_t __fastcall SceneLoader_DestroyMenuBuffer(
    std::uint32_t* self, void* /*edx*/)
{
    std::uint32_t result = CSF_AT(std::uint32_t, self, 0xA4);
    if (result >= 0x10)
    {
        result = CSF_AT(std::uint32_t, self, 0x90);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(std::uint32_t, self, 0xA4) = 15;
    CSF_AT(std::uint32_t, self, 0xA0) = 0;
    CSF_AT(std::uint8_t, self, 0x90) = 0;

    if (CSF_AT(std::uint32_t, self, 0x88) >= 0x10)
    {
        result = CSF_AT(std::uint32_t, self, 0x74);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(std::uint32_t, self, 0x88) = 15;
    CSF_AT(std::uint32_t, self, 0x84) = 0;
    CSF_AT(std::uint8_t, self, 0x74) = 0;

    if (CSF_AT(std::uint32_t, self, 0x6C) >= 0x10)
    {
        result = CSF_AT(std::uint32_t, self, 0x58);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(std::uint32_t, self, 0x6C) = 15;
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint8_t, self, 0x58) = 0;

    if (CSF_AT(std::uint32_t, self, 0x50) >= 0x10)
    {
        result = CSF_AT(std::uint32_t, self, 0x3C);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(std::uint32_t, self, 0x50) = 15;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint8_t, self, 0x3C) = 0;

    if (CSF_AT(std::uint32_t, self, 0x34) >= 0x10)
    {
        result = CSF_AT(std::uint32_t, self, 0x20);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }
    CSF_AT(std::uint32_t, self, 0x34) = 15;
    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint8_t, self, 0x20) = 0;

    return result;
}

HOOK(0x004FF3C0, SceneLoader_DestroyMenuBuffer, "auto");
