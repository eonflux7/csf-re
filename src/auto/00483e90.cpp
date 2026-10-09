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

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void* edx));
CSF_FUNC(int, __fastcall, ActionFsm_ResetSubObjectTarget, 0x004783A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CCharActionFsm_HandleDefaultAction, 0x0047FC20, (void* self, void* edx, int a2));

// Runs the action state update only while the action flag (field_0069) is set.
// If the active sub-object reports type 8, or the object in field_009c answers 11
// to its slot 0x4C query, the sub-object target is reset and state 27 is requested.
// Otherwise the default action handler runs.
static char __fastcall CCharActionFsm_UpdateActionStateWhenEnabled(CCharActionFsm* self, void* /*edx*/, int a2)
{
    char result = static_cast<char>(self->field_0069);
    if (result)
    {
        C_008319c0* subObject = reinterpret_cast<C_008319c0*>(self->vGetIndexedEntryOrZero());
        if (C_008319c0_GetSubObjectField74(subObject, nullptr) == 8)
        {
            goto ResetAndRequestState;
        }

        uint32_t activeObjectAddr = self->field_009c;
        if (activeObjectAddr != 0)
        {
            void* activeObject = reinterpret_cast<void*>(activeObjectAddr);
            int queryResult = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, activeObject, 0)[0x4C / 4])(activeObject, nullptr);
            if (queryResult == 0xB)
            {
                goto ResetAndRequestState;
            }
        }

        return CCharActionFsm_HandleDefaultAction(self, nullptr, a2);
    }
    return result;

ResetAndRequestState:
    {
        int targetHandle = self->vGetIndexedEntryOrZero();
        ActionFsm_ResetSubObjectTarget(reinterpret_cast<void*>(targetHandle), nullptr);
        return static_cast<char>(self->vSetStateAndDispatch(27));
    }
}

HOOK(0x483E90, CCharActionFsm_UpdateActionStateWhenEnabled, "auto");
