#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies four consecutive dwords from src into the focus-text RGBA colour block
// at this + 0x1C0 (CGuiButton::m_focusTextColor) and returns the last copied
// dword as a 32-bit int.
static int __fastcall CGuiButton_SetFocusTextColor(CGuiButton* self, void* /*edx*/, const uint32_t* src)
{
    uint32_t* dst = reinterpret_cast<uint32_t*>(&self->m_focusTextColor[0]);

    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    int result = static_cast<int>(src[3]);
    dst[3] = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x007252B0, CGuiButton_SetFocusTextColor, "auto");
