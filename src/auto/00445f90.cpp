#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void __fastcall Scene_ClearEmbeddedState(void* self, void* /*edx*/)
{
    auto* elements = CSF_AT(std::uint8_t*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = 0;
    CSF_AT(std::uint32_t, self, 0x8) = 0;

    if (elements)
    {
        void* block = elements - 4;
        const std::uint32_t count = CSF_AT(std::uint32_t, block, 0x0);
        if (static_cast<std::int32_t>(count - 1) >= 0)
        {
            auto* element = elements + count * 0x28 + 8;
            std::uint32_t remaining = count;
            do
            {
                element -= 0x28;
                --remaining;
                CSF_AT(void**, element, 0x0) = CSF_VTBL(0x0082D410);
            } while (remaining);
        }
        Mem_Free(block);
        CSF_AT(std::uint8_t*, self, 0x0) = nullptr;
    }
}

HOOK(0x00445F90, Scene_ClearEmbeddedState, "auto");
