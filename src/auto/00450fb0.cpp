#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Grows the animation record array of a CGuiControl (callee bound by address).
CSF_FUNC(void*, __fastcall, sub_44F990, 0x0044F990, (void* self, void* edx, int a2));

// Appends one 48-byte animation record to the array at +0xE8 and returns the new count.
// Array layout: +0xE8 base pointer, +0xEC capacity, +0xF0 count, +0xF4 grow size.
static int __fastcall CGuiControl_PushAnimRecord(CGuiControl* self, void* /*edx*/, const void* record)
{
    if (self->m_pPosAnim >= CSF_AT(uint32_t, self, 0xEC))
    {
        sub_44F990(&self->m_pColorAnim, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0xF4)));
    }

    uint32_t index = self->m_pPosAnim;
    uint8_t* slot = reinterpret_cast<uint8_t*>(self->m_pColorAnim) + 48 * index;
    self->m_pPosAnim = index + 1;
    std::memcpy(slot, record, 0x30);
    return static_cast<int>(index + 1);
}

HOOK(0x450FB0, CGuiControl_PushAnimRecord, "auto");
