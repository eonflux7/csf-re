#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// CGuiButton::SetEffects (0x00724F20)
// Stores the EFECTOS flag on the button and forwards it to the effects object at +0x1D4
// through virtual slot +0x20 (index 8). Returns the fx object's result, or the flag itself
// when no fx object exists.
//
// 00724F20  mov eax, [ebp+8]            ; value
// 00724F26  mov byte ptr [ecx+0x1BC], al; m_bEffects = (uint8_t)value
// 00724F2C  mov ecx, [ecx+0x1D4]        ; m_pFx
// 00724F32  test ecx, ecx / je  ...     ; skip when null
// 00724F36  mov edx, [ecx]              ; vtbl
// 00724F38  push eax                    ; full 32-bit value, not truncated
// 00724F39  call dword ptr [edx+0x20]   ; vtbl[8](value)
// 00724F3D  ret 4

static constexpr uint32_t kFxVtblSlot_Offset = 0x20;
static constexpr uint32_t kFxVtblSlot_Index  = kFxVtblSlot_Offset / 4;

static int __fastcall CGuiButton_SetEffects(CGuiButton* self, void* /*edx*/, int value)
{
    self->m_bEffects = static_cast<uint8_t>(value);

    uint32_t pFx = self->m_pFx;
    if (pFx)
    {
        void** vtbl = *reinterpret_cast<void***>(pFx);
        return ((int(__fastcall*)(void*, void*, int))vtbl[kFxVtblSlot_Index])(
            reinterpret_cast<void*>(pFx), nullptr, value);
    }
    return value;
}

HOOK(0x00724F20, CGuiButton_SetEffects, "auto");
