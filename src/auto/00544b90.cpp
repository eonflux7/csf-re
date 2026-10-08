#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Scheduler lookup: GetGlobalState_50D190() returns the global state, then
// SchedulerBase::GetSchedulerProxy is a __thiscall method taking that state as `this`.
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x50D190, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));

// Helper that clears the stats sub-object (cdecl, one argument, returns int*).
CSF_FUNC(int*, __cdecl, MenuSaveGame_ClearListContainer, 0x541650, (void* subobject));

// Layout forwarder. The original is __usercall with ebx carrying the first input;
// only the two stack arguments are expressible here (see notes).
CSF_FUNC(void, __cdecl, MenuMpStats_ForwardToLayout541230, 0x541570, (int a2, int a3));

// Refreshes the save-game menu layout after checking the scheduler proxy kind.
// `groupIndexEbx` is the original's ebx input, carried in the edx slot by the hook adapter.
static void __fastcall MenuSaveGame_RefreshLayoutForScheduler(void* self, uint32_t groupIndexEbx)
{
    int schedulerProxyKind = SchedulerBase_GetSchedulerProxy(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    if (schedulerProxyKind == 4)
    {
        // Virtual call on self->field_0058 (slot 13): arg = self->field_0024 + self->field_0054 * 200 + 80
        void* entryObject = CSF_AT(void*, self, 0x58);
        void** entryVtbl = *reinterpret_cast<void***>(entryObject);
        uint32_t entryIndex = CSF_AT(uint32_t, self, 0x54);
        uint32_t entryBase = CSF_AT(uint32_t, self, 0x24);
        int entryAddress = static_cast<int>(entryBase + entryIndex * 0xC8u + 0x50u);
        ((void(__fastcall*)(void*, void*, int))entryVtbl[13])(entryObject, nullptr, entryAddress);
    }
    else if (schedulerProxyKind != 8)
    {
        return;
    }

    CSF_AT(int, self, 0x50) = -1;

    void** selfVtbl = *reinterpret_cast<void***>(self);
    void* menuObject = ((void*(__fastcall*)(void*, void*))selfVtbl[8])(self, nullptr);

    void** menuVtbl = *reinterpret_cast<void***>(menuObject);
    ((void(__fastcall*)(void*, void*))menuVtbl[91])(menuObject, nullptr);

    void* statsSubobject = static_cast<uint8_t*>(self) + 0x20;
    MenuSaveGame_ClearListContainer(statsSubobject);

    int trailingValue = CSF_AT(int, self, 0x1C);
    MenuMpStats_ForwardToLayout541230(static_cast<int>(reinterpret_cast<uintptr_t>(statsSubobject)),
                                      trailingValue);
    (void)groupIndexEbx;
}

HOOK(0x00544B90, MenuSaveGame_RefreshLayoutForScheduler, "auto");
