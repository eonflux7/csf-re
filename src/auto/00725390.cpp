#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, CGuiControl_SetDraggable, 0x007180B0, (CGuiControl* self, void* , char value));
CSF_FUNC(int, __fastcall, CGuiControl_UpdateMouseHover, 0x00718DE0, (CGuiControl* self, void* , const std::uint32_t* pointerPosition, char mouseActive));
CSF_FUNC(int, __fastcall, CGuiControl_Construct_71A010, 0x0071A010, (CGuiControl* self, void* , uint32_t renderer, uint32_t context));
CSF_FUNC(int, __fastcall, CGuiButtonFx_Dispatch, 0x00731A00, (void* self, void* , int a2));
CSF_FUNC(uint32_t*, __fastcall, CGuiButtonFx_Construct, 0x00732920, (void* self, void* edx));

static int __fastcall CGuiButton_Construct(
    CGuiButton* self, void* /*edx*/, int renderer, int context)
{
    CGuiControl_Construct_71A010(self, nullptr, renderer, context);
    self->vtbl = CSF_VTBL(0x00865328);

    self->m_textColor[0] = 0.0f;
    self->m_textColor[1] = 0.0f;
    self->m_textColor[2] = 0.0f;
    self->m_textColor[3] = 1.0f;

    // Idle appearance.
    CSF_AT(float, self, 0x164) = 1.0f;
    CSF_AT(float, self, 0x158) = 0.0f;
    CSF_AT(float, self, 0x15C) = 0.0f;
    CSF_AT(float, self, 0x160) = 0.0f;
    self->m_pIdleQuad = 0;
    CSF_AT(std::uint8_t, self, 0x150) = 0;
    CSF_AT(float, self, 0x154) = -1.0f;
    CSF_AT(float, self, 0x158) = -1.0f;

    // Hover appearance.
    CSF_AT(float, self, 0x180) = 1.0f;
    CSF_AT(float, self, 0x174) = 0.0f;
    CSF_AT(float, self, 0x178) = 0.0f;
    CSF_AT(float, self, 0x17C) = 0.0f;
    CSF_AT(std::uint32_t, self, 0x168) = 0;
    CSF_AT(std::uint8_t, self, 0x16C) = 0;
    CSF_AT(float, self, 0x170) = -1.0f;
    CSF_AT(float, self, 0x174) = -1.0f;

    // Pressed appearance.
    CSF_AT(float, self, 0x19C) = 1.0f;
    CSF_AT(float, self, 0x190) = 0.0f;
    CSF_AT(float, self, 0x194) = 0.0f;
    CSF_AT(float, self, 0x198) = 0.0f;
    CSF_AT(std::uint32_t, self, 0x184) = 0;
    CSF_AT(std::uint8_t, self, 0x188) = 0;
    CSF_AT(float, self, 0x18C) = -1.0f;
    CSF_AT(float, self, 0x190) = -1.0f;

    // Permanent appearance.
    self->m_permanentTextColor[3] = 1.0f;
    self->m_permanentTextColor[0] = 0.0f;
    self->m_permanentTextColor[1] = 0.0f;
    self->m_permanentTextColor[2] = 0.0f;
    self->m_pPermanentQuad = 0;
    self->m_bPermanentTimed = 0;
    self->m_permanentTime = -1.0f;
    self->m_permanentTextColor[0] = -1.0f;

    self->m_focusTextColor[3] = 1.0f;
    self->m_focusTextColor[0] = 0.0f;
    self->m_focusTextColor[1] = 0.0f;
    self->m_focusTextColor[2] = 0.0f;

    self->m_state = 0;
    self->field_01e8 = 1;
    self->field_01d8 = 0.0f;
    self->field_01dc = 0.0f;

    std::uint32_t pointerPosition[2] = {
        static_cast<std::uint32_t>(-1),
        static_cast<std::uint32_t>(-1)
    };
    CGuiControl_UpdateMouseHover(self, nullptr, pointerPosition, 1);
    CGuiControl_SetDraggable(self, nullptr, 0);

    self->m_pText = 0;
    self->m_bEffects = 1;
    self->field_01ec = 1;
    self->m_bStateTimes = 0;
    self->m_focusTextColor[0] = -1.0f;
    self->field_01d0 = 1.0f;
    self->field_01e0 = 5;
    self->field_01e4 = 1069547520;

    void* storage = Mem_Alloc(0x20u);
    std::uint32_t* effects = storage
        ? CGuiButtonFx_Construct(storage, nullptr)
        : nullptr;
    self->m_pFx = reinterpret_cast<std::uint32_t>(effects);

    CGuiButtonFx_Dispatch(effects, nullptr, 0);
    CGuiButtonFx_Dispatch(reinterpret_cast<void*>(self->m_pFx), nullptr, 1);
    CGuiButtonFx_Dispatch(reinterpret_cast<void*>(self->m_pFx), nullptr, 2);
    CGuiButtonFx_Dispatch(reinterpret_cast<void*>(self->m_pFx), nullptr, 4);
    return reinterpret_cast<int>(self);
}

HOOK(0x00725390, CGuiButton_Construct, "auto");
