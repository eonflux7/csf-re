#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CCharWeaponFsm_ForwardWeaponAction(
    CCharWeaponFsm* self, void* /*edx*/, int action)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x198]
    }
}

static int __fastcall CCharWeaponFsm_DispatchWeaponAction(
    CCharWeaponFsm* self, void* /*edx*/, int action)
{
    const uint32_t state = self->field_000c;
    if (state == 1)
        return self->vCallVirtualMethod1E8(action);
    if (state == 8)
        return CCharWeaponFsm_ForwardWeaponAction(self, nullptr, action);
    return static_cast<int32_t>(state - 8);
}

HOOK(0x480110, CCharWeaponFsm_DispatchWeaponAction, "auto");
