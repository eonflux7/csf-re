#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, sub_4F23A0, 0x004F23A0, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall HudStringShow_ClearDebugOverlay(void* self, void* /*edx*/)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x08);
    uint32_t remaining = 0;
    if (begin)
    {
        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) - begin);
        remaining = static_cast<uint32_t>(byteCount / 124);
    }

    uint32_t offset = 0;
    while (remaining != 0)
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x08) + offset);
        void* object = CSF_AT(void*, entry, 0x78);
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            CSF_AT(uint32_t, entry, 0x78) = 0;
        }
        offset += 124;
        --remaining;
    }

    int result = CSF_AT(int, self, 0x08);
    if (result)
    {
        sub_4F23A0(result, CSF_AT(int, self, 0x0C));
        result = CSF_AT(int, self, 0x08);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    return result;
}

HOOK(0x004F2F90, HudStringShow_ClearDebugOverlay, "auto");
