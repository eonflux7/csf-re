#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiButton::SetStateTextColor
// Copies a 4-dword RGBA colour into the text-colour field of one of the four
// 28-byte state records based at +0x14C (idle/hover/pressed/permanent).
// Record layout: +0x00 quad*, +0x04 timed flag, +0x08 time, +0x0C text RGBA.
// The original computes the destination as (this + 0x158 + 28 * index) instead
// of dispatching on the index, so the same arithmetic is kept.
static int __fastcall CGuiButton_SetStateTextColor(CGuiButton* self, void* /*edx*/,
                                                   const uint32_t* src, int stateIndex)
{
    uint32_t* dest = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self) + 0x158 + 28 * stateIndex);

    uint32_t colorG = src[2];

    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = colorG;
    dest[3] = src[3];

    return static_cast<int>(colorG);
}

HOOK(0x00725280, CGuiButton_SetStateTextColor, "auto");
