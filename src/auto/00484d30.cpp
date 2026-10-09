#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CCharActionFsm_ResetActionSubstate, 0x004847C0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, IsSubFsmIdle, 0x004950F0, (void* self, void* edx));

static char __fastcall CCharActionFsm_CommitPendingAction(CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_00a4 == 0)
        return 0;

    CCharActionFsm_ResetActionSubstate(self, nullptr);

    uint32_t pendingAction = self->field_00a0;
    self->field_00a4 = 0;
    if (pendingAction != 0)
    {
        void* action = reinterpret_cast<void*>(pendingAction);
        self->field_009c = pendingAction;
        self->field_00a0 = 0;
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, action, 0)[12])(action, nullptr, 1);

        if (self->field_0024 != 0xFFFFFFFFu)
        {
            int slotObject = self->vGetIndexedEntryOrZero();
            void* slotPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(slotObject));
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotPtr, 0)[12])(slotPtr, nullptr, 1);
        }
    }

    if (self->vGetIndexedEntryOrZero())
    {
        int slotObject = self->vGetIndexedEntryOrZero();
        void* slotPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(slotObject));
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotPtr, 0)[12])(slotPtr, nullptr, 1);
    }

    if (IsSubFsmIdle(reinterpret_cast<void*>(self->field_0008), nullptr))
        return 1;

    void* weaponFsm = reinterpret_cast<void*>(self->field_0008);
    int slotResult = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, weaponFsm, 0)[27])(weaponFsm, nullptr, 0);
    return static_cast<char>(slotResult);
}

HOOK(0x00484D30, CCharActionFsm_CommitPendingAction, "auto");
