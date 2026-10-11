#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall GameBddLoader_ResetPending(void* self, void* /*edx*/)
{
    void* pending = CSF_AT(void*, self, 0xF0);
    if (!pending)
        return 0;

    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, pending, 0)[0x40 / 4])(pending, nullptr);

    CSF_AT(uint32_t, self, 0xF4) =
        CSF_AT(uint32_t, CSF_AT(void*, self, 0xF0), 0x78);
    CSF_AT(void*, self, 0xF0) = nullptr;
    return result;
}

HOOK(0x0050E1E0, GameBddLoader_ResetPending, "auto");
