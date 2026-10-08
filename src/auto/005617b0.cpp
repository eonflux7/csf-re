// CMenuMpSessions: clears a flag on the session-name slider and forwards it to its parent.
#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// int __thiscall sub_718660(void *this)  -- callee pops no stack args
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));

static int __fastcall CMenuMpSessions_ResetSliderFieldAndForward(CMenuMpSessions* self, void* /*edx*/)
{
    if (!self->field_003c)
        return 0;

    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_003c), 0x1C6) = 0;
    return CGuiSlider_ForwardSelfToParent(reinterpret_cast<void*>(self->field_003c), nullptr);
}

HOOK(0x005617B0, CMenuMpSessions_ResetSliderFieldAndForward, "auto");
