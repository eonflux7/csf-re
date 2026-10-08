#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __stdcall ResetStringStorage(std::uint8_t* storage)
{
    int result;

    if (CSF_AT(std::uint32_t, storage, 0x1C) >= 8u)
    {
        result = CSF_AT(std::int32_t, storage, 0x08);
        if (result != 0)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(
                    static_cast<std::uint32_t>(result))));
        }
    }

    CSF_AT(std::uint32_t, storage, 0x1C) = 7;
    CSF_AT(std::uint32_t, storage, 0x18) = 0;
    CSF_AT(std::uint16_t, storage, 0x08) = 0;

    return result;
}

HOOK(0x005466C0, ResetStringStorage, "auto");
