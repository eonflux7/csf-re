#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Calls the child object's virtual slot +0x4C (no args) and returns its int result.
// Child object is at CCharWeaponFsm +0x9C; its type is not declared, so it is used via a raw vtable slot.
static bool __fastcall CCharWeaponFsm_ProcessChildStateTen(CCharWeaponFsm* self, void* /*edx*/)
{
    void* childObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x9C));
    if (childObject == nullptr)
    {
        return false;
    }

    int childState = ((int(__fastcall*)(void*, void*))CSF_AT(void**, childObject, 0)[0x4C / 4])(childObject, nullptr);
    if (childState != 10)
    {
        return false;
    }

    self->vUpdateWeaponSelection(12);
    return true;
}

HOOK(0x00484590, CCharWeaponFsm_ProcessChildStateTen, "auto");
