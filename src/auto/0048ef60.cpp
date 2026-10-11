#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GuiControl_SetInteractionState, 0x0048EEC0, (void* self, void* edx, int a2));

static void __fastcall CPlayerCharacter_SetChildEnabled(
    CPlayerCharacter* self, void* /*edx*/, char enabled)
{
    const int interactionState = enabled == 0;
    GuiControl_SetInteractionState(self, nullptr, interactionState);
    self->vSlot6C(interactionState);
}

HOOK(0x0048EF60, CPlayerCharacter_SetChildEnabled, "auto");
