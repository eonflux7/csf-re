#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t GfxAnimCompress_ExpandComponentBits(uint16_t packed)
{
    const uint32_t value = packed;
    uint32_t bits = (value & 0x8000) << 16;
    if ((value & 0x7FFF) != 0)
    {
        bits = ((value & 0x7FF) << 12)
             | (((value & 0x7800) + 229376) << 12)
             | ((value & 0x8000) << 16);
    }
    return bits;
}

static uint32_t __cdecl GfxAnimCompress_UnpackFourComponents(
    hook::Regs* regs, const uint32_t* /*args*/)
{
    const uint32_t originalEax = regs->eax;
    const void* source = reinterpret_cast<const void*>(originalEax);
    void* destination = reinterpret_cast<void*>(regs->esi);

    CSF_AT(uint32_t, destination, 0x00) =
        GfxAnimCompress_ExpandComponentBits(CSF_AT(uint16_t, source, 0x08));
    CSF_AT(uint32_t, destination, 0x04) =
        GfxAnimCompress_ExpandComponentBits(CSF_AT(uint16_t, source, 0x0A));
    CSF_AT(uint32_t, destination, 0x08) =
        GfxAnimCompress_ExpandComponentBits(CSF_AT(uint16_t, source, 0x0C));

    const uint16_t lastComponent = CSF_AT(uint16_t, source, 0x0E);
    const uint32_t lastBits = GfxAnimCompress_ExpandComponentBits(lastComponent);
    CSF_AT(uint32_t, destination, 0x0C) = lastBits;

    return (lastComponent & 0x7FFF) != 0 ? lastBits : originalEax;
}

HOOK_REGS(0x006C24B0, GfxAnimCompress_UnpackFourComponents, 0, "gfx_anim_compress");
