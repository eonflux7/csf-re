#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_CheckActionPrecondition, 0x00485A70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsSubObjectFieldFlagSet, 0x00760FF0, (int a1));

// Tries to start the pending action when no item is in use and the sub-object flag allows it.
// Returns the value of the last check, or the result of the dispatch call on the sub-object.
static int __fastcall CCharActionFsm_TryStartPendingAction(CCharActionFsm* self, void* /*edx*/)
{
    int result = self->vSlotD8();
    void* firstObject = reinterpret_cast<void*>(result);
    if (CSF_AT(uint32_t, firstObject, 0x70) != 0)
        return result;

    void* itemOwner = reinterpret_cast<void*>(self->vSlotD8());
    result = GetCurrentItemCount(itemOwner, nullptr);
    if (result == 0 && self->field_0028 == 0xFFFFFFFFu) {
        C_008319c0* subObject = reinterpret_cast<C_008319c0*>(self->vSlotD8());
        int field74Value = C_008319c0_GetSubObjectField74(subObject, nullptr);
        result = IsSubObjectFieldFlagSet(field74Value);
        if (static_cast<uint8_t>(result) != 0) {
            result = CCharActionFsm_CheckActionPrecondition(self, nullptr);
            if (result != 0) {
                void* actionObject = reinterpret_cast<uint8_t*>(self->field_0008) + 4;
                uint32_t callArgs[2] = {1u, 0xFFFFFFFFu};
                using DispatchFn = int(__fastcall*)(void*, void*, uint32_t*);
                auto dispatch = reinterpret_cast<DispatchFn>(CSF_AT(void**, actionObject, 0)[0x68 / 4]);
                return dispatch(actionObject, nullptr, callArgs);
            }
        }
    }
    return result;
}

HOOK(0x004864D0, CCharActionFsm_TryStartPendingAction, "auto");
