#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CCharWeaponFsm_TailDispatchAction(
    CCharWeaponFsm* /*action*/, void* /*edx*/, int /*argument*/)
{
    __asm {
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0xb4]
    }
}

static int __fastcall CCharWeaponFsm_ForwardToActiveWeaponAction(
    CCharWeaponFsm* self, void* /*edx*/, int argument)
{
    const int action = self->vSlotD8();
    if (!action)
        return 0;

    [[clang::musttail]] return CCharWeaponFsm_TailDispatchAction(
        reinterpret_cast<CCharWeaponFsm*>(static_cast<std::uintptr_t>(action)),
        nullptr, argument);
}

HOOK(0x00480C40, CCharWeaponFsm_ForwardToActiveWeaponAction, "auto");
