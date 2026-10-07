#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns 1 if the base control's virtual at vtable slot +0xE4 reports shown/active,
// otherwise 1 if this panel owns a background quad, else 0.
static int __fastcall CGuiPanel_IsShownOrHasBackground(CGuiPanel* self, void* /*edx*/)
{
    void** vtbl = CSF_AT(void**, self, 0);
    uint8_t isShown = ((uint8_t(__fastcall*)(void*, void*))vtbl[0xE4 / 4])(self, nullptr);
    if (isShown != 0)
        return 1;
    if (self->m_pBackgroundQuad != 0)
        return 1;
    return 0;
}

HOOK(0x0072BC90, CGuiPanel_IsShownOrHasBackground, "auto");
