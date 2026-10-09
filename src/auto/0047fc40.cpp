#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CCharWeaponFsm_TailDispatchState5(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1d0]
    }
}

static __declspec(naked) int __fastcall CCharWeaponFsm_TailDispatchState6(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1d4]
    }
}

static __declspec(naked) int __fastcall CCharWeaponFsm_TailDispatchState7(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1d8]
    }
}

static __declspec(naked) int __fastcall CCharWeaponFsm_TailDispatchState8(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1dc]
    }
}

static int __fastcall CCharWeaponFsm_DispatchStateTransition(
    CCharWeaponFsm* self, void* edx, int argument)
{
    const uint32_t state = self->field_0010;
    switch (state)
    {
    case 5u:
        return CCharWeaponFsm_TailDispatchState5(self, edx, argument);
    case 6u:
        return CCharWeaponFsm_TailDispatchState6(self, edx, argument);
    case 7u:
        return CCharWeaponFsm_TailDispatchState7(self, edx, argument);
    case 8u:
        return CCharWeaponFsm_TailDispatchState8(self, edx, argument);
    default:
        return static_cast<int32_t>(state - 5u);
    }
}

HOOK(0x47FC40, CCharWeaponFsm_DispatchStateTransition, "auto");
