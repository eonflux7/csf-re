#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Fallback action handler at 0x47FE80, called when no special action applies.
CSF_FUNC(char, __fastcall, sub_47FE80, 0x0047FE80, (void* self, void* edx, int a2));

// Slot 0x4C of the object at field_009c returns its state (__thiscall, this in ecx).
using ActionStateGetter = int(__fastcall*)(void* self, void* edx);

// Returns the enabled flag (field_0069). When it is set and the secondary object at
// field_009c reports state 10 via its vtable slot 0x4C, dispatches through the
// CCharWeaponFsm vSlot138 wrapper with argument 4. Otherwise runs the fallback action.
static char __fastcall CCharActionFsm_DispatchActionIfEnabled(CCharActionFsm* self, void* /*edx*/, int a2)
{
    char result = static_cast<char>(self->field_0069);
    if (result)
    {
        void* actionObj = reinterpret_cast<void*>(self->field_009c);
        if (actionObj)
        {
            ActionStateGetter getState = reinterpret_cast<ActionStateGetter>(CSF_AT(void**, actionObj, 0)[0x4C / 4]);
            if (getState(actionObj, nullptr) == 10)
            {
                return static_cast<char>(self->vSlot138(4));
            }
        }
        return sub_47FE80(self, nullptr, a2);
    }
    return result;
}

HOOK(0x483D40, CCharActionFsm_DispatchActionIfEnabled, "auto");
