#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetPosition, 0x007198A0,
         (CGuiControl* self, void* edx, float x, float y));

// CGuiSwitch::SetPosition(float x, float y)
// Forwards to the base CGuiControl implementation, then, when the switch owns a
// text object (m_pText), forwards the requested position to it through vtable
// slot +0x98 (index 0x26). Returns 0 when there is no text object.
static int __fastcall CGuiSwitch_SetPosition(
    CGuiSwitch* self, void* /*edx*/, float x, float y)
{
    CGuiControl_SetPosition(self, nullptr, x, y);

    if (self->m_pText == 0)
        return 0;

    void* textObject = reinterpret_cast<void*>(self->m_pText);
    float position[2] = { x, y };

    return reinterpret_cast<int (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, textObject, 0)[0x98 / 4])(
            textObject, nullptr, position);
}

HOOK(0x007304E0, CGuiSwitch_SetPosition, "auto");
