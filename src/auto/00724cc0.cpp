#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x731A00: method of the button's 0x20-byte effect object (built by 0x732920),
// reached via the tail jump with ecx = button->m_pFx and the int argument forwarded.
CSF_FUNC(int, __fastcall, CGuiButtonFx_Dispatch, 0x731A00, (void* self, void* /*edx*/, int a2));

// 0x724CC0: CGuiButton method. If the effect object exists, forward the int argument to it;
// otherwise do nothing.
static int __fastcall CGuiButton_DispatchToFx(CGuiButton* self, void* /*edx*/, int a2)
{
    uint32_t pFx = self->m_pFx;
    if (pFx == 0)
        return 0;                       // original leaves eax untouched on this path
    return CGuiButtonFx_Dispatch(reinterpret_cast<void*>(pFx), nullptr, a2);
}

HOOK(0x724CC0, CGuiButton_DispatchToFx, "auto");
