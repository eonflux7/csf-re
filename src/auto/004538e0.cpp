#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a buffer of 0x18-byte elements: every element gets vtable C_0082d410 at +0 and +0xC,
// then the buffer is freed with Mem_FreeDebug and the begin/end/capacity fields are zeroed.
static uint32_t* __fastcall ResetElementArrayAndFree(void* self, void* /*edx*/)
{
    int lastFreeResult = 0;

    uint32_t beginAddr = CSF_AT(uint32_t, self, 0x4);
    if (beginAddr != 0)
    {
        uint32_t endAddr = CSF_AT(uint32_t, self, 0x8);
        for (uint32_t cur = beginAddr; cur != endAddr; cur += 0x18)
        {
            uint8_t* element = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(cur));
            CSF_AT(void*, element, 0xC) = CSF_VTBL(0x0082D410);
            CSF_AT(void*, element, 0x0) = CSF_VTBL(0x0082D410);
        }

        beginAddr = CSF_AT(uint32_t, self, 0x4);
        if (beginAddr != 0)
        {
            void* block = reinterpret_cast<void*>(static_cast<uintptr_t>(beginAddr));
            lastFreeResult = Mem_FreeDebug(block);
        }
    }

    CSF_AT(uint32_t, self, 0x4) = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;

    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(lastFreeResult));
}

HOOK(0x004538E0, ResetElementArrayAndFree, "auto");
