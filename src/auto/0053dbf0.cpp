#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Original calls: GetGlobalState_50D190 (0x50D190, __cdecl, no args) -> scheduler base in eax.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, (void));

// Concurrency::details::SchedulerBase::GetSchedulerProxy (0x734A10, __thiscall, ecx = scheduler base).
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

// sub_53D740 (0x53D740): usercall, ecx = this, edi = a2 (forwarded from the caller), one stack arg, ret 4.
// Bound here as __fastcall with the stack argument in position 3; see notes regarding edi.
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x0053D740,
         (CMenuMissionInfo* self, void* edx, int flag));

// sub_53DAA0 (0x53DAA0): __thiscall void, no stack args.
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ResetPanels, 0x0053DAA0,
         (CMenuMissionInfo* self, void* edx));

// Closes the active mission-info panel when the scheduler proxy reports value 16.
static void __fastcall CMenuMissionInfo_ClosePanelAndDeactivate(CMenuMissionInfo* self, void* /*edx*/)
{
    if (self->field_00a6 == 0)
        return;

    void* schedulerBase = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    if (SchedulerBase_GetSchedulerProxy(schedulerBase, nullptr) != 16)
        return;

    CMenuMissionInfo_EnterPanelState(self, nullptr, 0);

    // Re-read the child object pointer after the call above.
    void* childObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_001c));
    self->field_0014 = 0;

    // childObject->vtbl[0xA8 / 4](childObject, 1)
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, childObject, 0)[0xA8 / 4])(childObject, nullptr, 1);

    if (self->field_00a4 != 0)
    {
        // self->vtbl[0x48 / 4](self)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr);
    }
    else
    {
        CMenuMissionInfo_ResetPanels(self, nullptr);
    }

    self->field_00a6 = 0;
}

HOOK(0x0053DBF0, CMenuMissionInfo_ClosePanelAndDeactivate, "auto");
