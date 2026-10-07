#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiControl_SetPosition, 0x007198A0,
         (CGuiControl* self, void* edx, float x, float y));

static int __fastcall CGuiEdit_SetPosition(
    CGuiEdit* self, void* /*edx*/, float x, float y)
{
    CGuiControl_SetPosition(self, nullptr, x, y);

    if (self->m_pText == 0)
        return 0;

    void* textObject = reinterpret_cast<void*>(self->m_pText);
    float position[2] = {x, y};

    return reinterpret_cast<int (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, textObject, 0)[0x98 / 4])(
            textObject, nullptr, position);
}

HOOK(0x00725BE0, CGuiEdit_SetPosition, "auto");
