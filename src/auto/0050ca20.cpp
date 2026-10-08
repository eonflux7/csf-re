#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees the buffer held at +0xC and clears the +0xC/+0x10/+0x14 members.
static int __fastcall FreeBufferAndClear(int* self, void* /*edx*/)
{
    int result = *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self) + 0xC);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<std::uintptr_t>(result)));

    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0xC) = 0;
    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0x10) = 0;
    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(self) + 0x14) = 0;
    return result;
}

HOOK(0x0050CA20, FreeBufferAndClear, "auto");
