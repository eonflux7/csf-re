#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall FreeAndResetBuffer(int* self, void* /*edx*/)
{
    void* block = CSF_AT(void*, self, 0x19C);
    int result = 0;
    if (block)
        result = Mem_FreeDebug(block);

    CSF_AT(std::uint32_t, self, 0x19C) = 0;
    CSF_AT(std::uint32_t, self, 0x1A0) = 0;
    CSF_AT(std::uint32_t, self, 0x1A4) = 0;
    return result;
}

HOOK(0x007158E0, FreeAndResetBuffer, "auto");
