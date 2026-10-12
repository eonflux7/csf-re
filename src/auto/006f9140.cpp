#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Allocates and initializes a 0x80-byte screen-effect object (OldFilm/Noise/FadeIn/Oscil family).
// Returns nullptr when the allocation fails.
static C_00860178* __cdecl GfxScreenEffects_CreateEffectObject()
{
    void* allocated = Mem_Alloc(128);
    if (allocated == nullptr)
        return nullptr;

    C_00860178* effect = static_cast<C_00860178*>(allocated);
    C_00860178_Construct(effect, nullptr);

    CSF_AT(void*, effect, 0x04) = CSF_VTBL(0x0082D428);
    CSF_AT(std::uint8_t, effect, 0x08) = 0;
    CSF_AT(std::uint32_t, effect, 0x54) = 0;
    CSF_AT(std::uint32_t, effect, 0x58) = 0;
    CSF_AT(std::uint32_t, effect, 0x5C) = 0;
    CSF_AT(void*, effect, 0x00) = CSF_VTBL(0x008629F8);
    CSF_AT(void*, effect, 0x04) = CSF_VTBL(0x008629DC);
    CSF_AT(std::uint32_t*, effect, 0x78) = RwMatrixCreate();
    CSF_AT(std::uint8_t, effect, 0x7C) = 1;

    return effect;
}

HOOK(0x6F9140, GfxScreenEffects_CreateEffectObject, "auto");
