#include "hook.h"
#include "types/auto_classes.h"

// Returns the delegate pointer the original would tail-jump through, or 0.
// Mirrors the original's two slot +0xD8 getter calls: the first decides whether
// a delegate exists, the second supplies the delegate itself.
static int __fastcall CCharWeaponFsm_GetDelegateOrZero(CCharWeaponFsm* self, void* /*edx*/)
{
    int firstResult = self->vSlotD8();
    if (firstResult == 0)
        return 0;
    return self->vSlotD8();
}

// Whole-function naked thunk. The tail jmp must run with the original caller's
// return address and stack argument in place, so there are no pushes or frame.
// The getter calls happen inside the helper above, which returns with the stack
// restored; ecx still holds `this` when the helper is entered.
static __declspec(naked) void CCharWeaponFsm_ForwardToDelegateOrReturnZero()
{
    __asm {
        lea eax, CCharWeaponFsm_GetDelegateOrZero
        call eax
        test eax, eax
        je NoDelegate
        mov ecx, eax
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x30]
NoDelegate:
        ret 4
    }
}

HOOK(0x480C10, CCharWeaponFsm_ForwardToDelegateOrReturnZero, "auto");
