#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CMenuMessageBox button/state dispatcher @ 0x0052B9A0 (20 bytes).
//
// Original code:
//     mov  al, byte ptr [ecx + 0x8AC]   ; CMenuMessageBox::field_08ac
//     test al, al
//     jne  flag_set
//     jmp  0x528AE0                     ; field_08ac == 0
// flag_set:
//     jmp  0x52B650                     ; field_08ac != 0
//
// The original never returns and pops no stack bytes: both branches are pure
// tail jumps to real callees, so each branch is reproduced as a tail call
// using the callee's own convention:
//     0x528AE0 - std::uint32_t* __thiscall(CMenuMessageBox* this)         (input: ecx)
//     0x52B650 - int __usercall(int a1@<ecx>, int a2@<ebx>)        (inputs: ecx, ebx)
// ebx (a2 of 0x52B650) is forwarded implicitly: the original merely jumps on
// and leaves ebx untouched, and this dispatcher's code never touches ebx
// either. edx is no callee's input and is forwarded unchanged, exactly as the
// original (which never touches edx) does. No stack arguments are involved.

CSF_FUNC(int, __fastcall, MenuMessageBox_CollapseLayout, 0x528AE0, (CMenuMessageBox* self, void* /*edx*/));
CSF_FUNC(int, __fastcall, CMenuMessageBox_RefreshLayoutAndButtons, 0x52B650, (CMenuMessageBox* self, void* /*edx*/));

static int __fastcall CMenuMessageBox_DispatchByStateFlag(CMenuMessageBox* self, void* edx)
{
    if (self->field_08ac != 0)
        return CMenuMessageBox_RefreshLayoutAndButtons(self, edx);

    return MenuMessageBox_CollapseLayout(self, edx);
}

HOOK(0x52B9A0, CMenuMessageBox_DispatchByStateFlag, "auto");
