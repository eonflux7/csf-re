#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CCharWeaponFsm_ForwardFirstStateHandler(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c0]
    }
}

static __declspec(naked) int __fastcall CCharWeaponFsm_ForwardSecondStateHandler(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c4]
    }
}

static __declspec(naked) int __fastcall CCharWeaponFsm_ForwardThirdStateHandler(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c8]
    }
}

static __declspec(naked) int __fastcall CCharWeaponFsm_ForwardFourthStateHandler(
    CCharWeaponFsm*, void*, int)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1cc]
    }
}

static int __fastcall CCharWeaponFsm_DispatchStateVirtual(
    CCharWeaponFsm* self, void* edx, int argument)
{
    const uint32_t state = self->field_0010;
    switch (state)
    {
    case 1u:
        return CCharWeaponFsm_ForwardFirstStateHandler(self, edx, argument);
    case 2u:
        return CCharWeaponFsm_ForwardSecondStateHandler(self, edx, argument);
    case 3u:
        return CCharWeaponFsm_ForwardThirdStateHandler(self, edx, argument);
    case 4u:
        return CCharWeaponFsm_ForwardFourthStateHandler(self, edx, argument);
    default:
        return static_cast<int>(state - 1);
    }
}

HOOK(0x0047FC90, CCharWeaponFsm_DispatchStateVirtual, "auto");
