// CMenuMessageBox sub_52B960
// Dispatcher on the message-box "action" selector stored at +0x28:
//   0 -> tail-jump into 0x528C20
//   1 -> plain call into 0x52B650, then fall through to the epilogue
//   2 -> tail-jump into 0x52B930
//   3 -> tail-jump into 0x52B770
//   >3 -> return immediately (unsigned "above" test, i.e. ja on the value)
// The float a3 stays on the stack and is forwarded unchanged to the handlers,
// so this function itself pops 4 bytes (ret 4).

#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x528C20 - selector 0 handler: plain __thiscall, float arg on stack (pops 4).
CSF_FUNC(void, __fastcall, CMenuMessageBox_UpdateOpeningAnimation, 0x00528C20,
         (CMenuMessageBox* self, void* /*edx*/, float a3));

// 0x52B650 - selector 1 handler: second arg in ebx, returns int, pops nothing (ret 0).
CSF_FUNC(int, __fastcall, CMenuMessageBox_RefreshLayoutAndButtons, 0x0052B650,
         (CMenuMessageBox* self, void* arg_ebx));

// 0x52B930 - selector 2 handler: second arg in ebx plus float on stack (pops 4).
CSF_FUNC(unsigned int, __fastcall, MenuMessageBox_IncrementPhaseCounter, 0x0052B930,
         (CMenuMessageBox* self, void* arg_ebx, float a3));

// 0x52B770 - selector 3 handler: second arg in ebx plus float on stack (pops 4).
CSF_FUNC(int, __fastcall, CMenuMessageBox_UpdateTimersAndSelection, 0x0052B770,
         (CMenuMessageBox* self, void* arg_ebx, float a3));

static void __fastcall CMenuMessageBox_DispatchAction(CMenuMessageBox* self,
                                                      void* arg_ebx,
                                                      float a3)
{
    // Original reads the selector once and does an unsigned range check (cmp/ja).
    switch (self->field_0028)
    {
    case 0:
        CMenuMessageBox_UpdateOpeningAnimation(self, nullptr, a3);
        return;
    case 1:
        // Return value is unused by the original (just a call + fallthrough).
        CMenuMessageBox_RefreshLayoutAndButtons(self, arg_ebx);
        return;
    case 2:
        MenuMessageBox_IncrementPhaseCounter(self, arg_ebx, a3);
        return;
    case 3:
        CMenuMessageBox_UpdateTimersAndSelection(self, arg_ebx, a3);
        return;
    default:
        return;
    }
}

HOOK(0x0052B960, CMenuMessageBox_DispatchAction, "auto");
