#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x718300: __thiscall, pops 1 stack arg. Declared with an int result so the value the callee
// leaves in eax is forwarded unchanged (the target is a plain tail pass-through of that call).
CSF_FUNC(int, __fastcall, CGuiListBoxBase_NotifyAction, 0x718300,
         (CGuiListBoxBase* self, void* edx, int actionId));

// Forwards the incoming action id to the attached slider (m_pSlider, if present) through the
// slider's vtable slot +0x14, then passes it on to the list box notification handler.
static int __fastcall CGuiListBoxBase_ForwardCommandToSlider(
    CGuiListBoxBase* self, void* /*edx*/, int actionId)
{
    void* slider = reinterpret_cast<void*>(self->m_pSlider);
    if (slider)
    {
        typedef void (__fastcall *SliderCommandFn)(void* self, void* edx, int actionId);
        SliderCommandFn command =
            reinterpret_cast<SliderCommandFn>(CSF_AT(void**, slider, 0)[0x14 / 4]);
        command(slider, nullptr, actionId);
    }

    return CGuiListBoxBase_NotifyAction(self, nullptr, actionId);
}

HOOK(0x714120, CGuiListBoxBase_ForwardCommandToSlider, "auto");
