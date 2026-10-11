#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, ApplyColorAnimationToControl, 0x00445050, (uint32_t* self, void* , C_00833908* animation, uint32_t* entry));

static int __fastcall C_0083e160_ApplyColorAnimIfEnabled(C_0083e160* self, void* /*edx*/, uint32_t* a2)
{
    self->field_0014 = a2[5];
    self->field_0010 = a2[9];

    bool animationEnabled = self->vSlotB0();
    if (!animationEnabled)
        return 0;

    C_00833908* animation = reinterpret_cast<C_00833908*>(self->field_000c);
    CGuiControl* guiControl = reinterpret_cast<CGuiControl*>(GetGlobalValue_8B0568());
    uint32_t* colorAnim = reinterpret_cast<uint32_t*>(CGuiControl_GetColorAnim(guiControl, nullptr));
    return ApplyColorAnimationToControl(colorAnim, nullptr, animation, a2);
}

HOOK(0x004E1180, C_0083e160_ApplyColorAnimIfEnabled, "auto");
