#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Selects the weaponFsm state (1..9), clears the per-state flag and timer,
// then dispatches to the matching virtual handler. Unknown states return a2 - 1.
static int __fastcall CCharWeaponFsm_SetStateAndDispatch(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    self->field_0010 = static_cast<uint32_t>(a2);
    int result = a2 - 1;
    self->field_0069 = 1;
    self->field_006c = 0.0f;

    switch (a2)
    {
    case 1:
        result = self->vRefreshWeaponStateAndSetMode15();
        break;
    case 2:
        result = self->vInvokeVtblSlot71With14();
        break;
    case 3:
        result = self->vRefreshWeaponStateAndSetMode15_69();
        break;
    case 4:
        result = self->vInvokeVtblSlot71With16();
        break;
    case 5:
        // Slot 0x1AC has no typed wrapper.
        result = ((int(__fastcall*)(void*, void*))self->vtbl[0x1AC / 4])(self, nullptr);
        break;
    case 6:
        result = self->vArg9();
        break;
    case 7:
        result = self->vDispatchWhenCheckPasses();
        break;
    case 8:
        result = self->vPrepareAndTailDispatch();
        break;
    case 9:
        result = self->vSetSubobjectFlagThenQueryState15();
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x480570, CCharWeaponFsm_SetStateAndDispatch, "auto");
