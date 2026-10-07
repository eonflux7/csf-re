#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __thiscall CGuiControl_SetDisabled(CGuiControl* this, char a2)  (ret 4)
CSF_FUNC(int, __fastcall, CGuiControl_SetDisabled, 0x718080, (CGuiControl* self, void* edx, char a2));

// Thunk: the original only sets up a dummy frame (push ebp / mov ebp,esp / pop ebp) and
// tail-jumps to CGuiControl_SetDisabled, forwarding `this` and the single stack argument.
static int __fastcall CGuiPanel_SetDisabled(CGuiPanel* self, void* /*edx*/, char a2)
{
    return CGuiControl_SetDisabled(self, nullptr, a2);
}

HOOK(0x72A580, CGuiPanel_SetDisabled, "auto");
