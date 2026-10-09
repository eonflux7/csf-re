#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsWeaponSubObjectReady, 0x00478280, (void* self, void* edx));

static CCharWeaponFsm* __fastcall CCharWeaponFsm_PrepareWeaponForAction(
    CCharWeaponFsm* self, void* /*edx*/)
{
    void* weapon = reinterpret_cast<void*>(self->vSlotD8());
    if (!IsWeaponSubObjectReady(weapon, nullptr)) {
        void* subObject = reinterpret_cast<void*>(self->vSlotD8());
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, subObject, 0)[0xDC / 4])(subObject, nullptr, 0);
    }

    self->vSlot11C(22);
    return self;
}

static CCharWeaponFsm* (__fastcall* s_prepareWeaponForAction)(
    CCharWeaponFsm*, void*) = &CCharWeaponFsm_PrepareWeaponForAction;

static __declspec(naked) void CCharWeaponFsm_PrepareAndTailDispatch()
{
    __asm {
        mov eax, dword ptr [s_prepareWeaponForAction]
        call eax
        mov ecx, eax
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x1e4]
    }
}

HOOK(0x004806C0, CCharWeaponFsm_PrepareAndTailDispatch, "auto");
