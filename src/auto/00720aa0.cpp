#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_ResetInput, 0x00717F70,
         (CGuiControl* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiControl_ForwardEventToFirstEffect, 0x00718C00,
         (CGuiControl* self, void* edx, int valueBits));

static int __fastcall CGuiStatic_ResetState(CGuiStatic* self, void* /*edx*/)
{
    CGuiControl_ResetInput(self, nullptr);

    self->field_0140 = 1.0f;
    CGuiControl_ForwardEventToFirstEffect(self, nullptr, 0x3F800000);

    void* text = reinterpret_cast<void*>(self->m_pText);
    self->field_0144 = 0;
    if (text)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, text, 0)[0xB4 / 4])(text, nullptr);
    }

    self->field_015c = 0xFFFFFFFFu;
    self->field_0158 = 0xFFFFFFFFu;
    return -1;
}

HOOK(0x00720AA0, CGuiStatic_ResetState, "auto");
