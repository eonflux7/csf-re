#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, MinimapRenderer_InitSmall, 0x0043ABA0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, MinimapRenderer_InitLarge, 0x0043AD40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Selector 1: builds the large renderer (92 bytes). The original tail-jumps to 0x43DB60,
// whose body is inlined here (it holds the 0x830E5C vtable store).
// Selector 0: builds the 80- or 76-byte renderer depending on the global flag at +0xD0.
// Any other selector returns nullptr.
static uint32_t* Minimap_CreateRenderer(int selector)
{
    if (selector == 0)
    {
        std::uint8_t* globalState =
            reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(GetGlobalState_50D190()));

        if (CSF_AT(std::uint8_t, globalState, 0xD0) != 0)
        {
            uint32_t* renderer = static_cast<uint32_t*>(Mem_Alloc(0x50));
            if (renderer != nullptr)
            {
                MinimapRenderer_InitSmall(renderer, nullptr);
                *reinterpret_cast<void***>(renderer) = CSF_VTBL(0x00830DF8);
                CSF_AT(uint32_t, renderer, 0x48) = 0;
                return renderer;
            }
        }
        else
        {
            uint32_t* renderer = static_cast<uint32_t*>(Mem_Alloc(0x4C));
            if (renderer != nullptr)
            {
                MinimapRenderer_InitSmall(renderer, nullptr);
                *reinterpret_cast<void***>(renderer) = CSF_VTBL(0x00830DC8);
                CSF_AT(uint32_t, renderer, 0x48) = 0;
                return renderer;
            }
        }
        return nullptr;
    }

    if (selector == 1)
    {
        uint32_t* renderer = static_cast<uint32_t*>(Mem_Alloc(0x5C));
        if (renderer == nullptr)
        {
            return nullptr;
        }

        MinimapRenderer_InitLarge(renderer, nullptr);
        *reinterpret_cast<void***>(renderer) = CSF_VTBL(0x00830E5C);
        CSF_AT(uint32_t, renderer, 0x50) = 0;
        CSF_AT(uint32_t, renderer, 0x54) = 0;
        CSF_AT(uint32_t, renderer, 0x58) = 0;
        return renderer;
    }

    return nullptr;
}

HOOK(0x0043B450, Minimap_CreateRenderer, "auto");
