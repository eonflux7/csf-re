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

// Concurrency::details::SchedulerBase::GetSchedulerProxy (thiscall, no stack args).
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x734A10, (void* self, void* edx));

// Global scheduler state accessor (cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());

// Refresh selection and add item (usercall: ecx=this, ebx=a2 passed through by the caller).
CSF_FUNC(int, __fastcall, MenuControls_RefreshSelectionAndAddItem, 0x542C90, (CMenuControls* self, void* edx));

// Config row builder (cdecl, 4 stack args, returns pointer in eax).
CSF_FUNC(void*, __cdecl, MenuControls_BuildConfigEntries, 0x42AC60, (int a1, int a2, int a3, void* a4));

// Config row binder (usercall in the original: esi=this, ebx=caller's ebx; stack args a3..a5, callee does not pop).
CSF_FUNC(void, __cdecl, MenuControls_BindConfigEntry, 0x42AF90, (int a3, void* a4, uint32_t a5));

// Listbox add-selected-item (thiscall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x716A60, (void* self, void* edx, int a2));

// Body of the apply path that the original reaches via a tail jmp to 0x542EB0.
// Reproduced inline because 0x542EB0 is not a function start in the binary.
static int CMenuControls_ApplySelectedConfiguration(CMenuControls* self)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);

    MenuControls_BuildConfigEntries(
        static_cast<int>(self->field_02d4),
        static_cast<int>(self->field_02cc),
        static_cast<int>(self->field_02d0),
        static_cast<void*>(selfBytes + 0x48));

    MenuControls_BindConfigEntry(
        static_cast<int>(self->field_02c8),
        static_cast<void*>(selfBytes + 0x48),
        self->field_002c);

    int addResult = CGuiListBoxBase_AddSelectedItem(
        reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_002c)),
        nullptr,
        static_cast<int>(self->field_02d8));

    self->field_02dc = 3;
    return addResult;
}

static int __fastcall CMenuControls_StepStateMachine(CMenuControls* self, void* /*edx*/)
{
    uint32_t state = self->field_02dc;

    if (state == 0)
    {
        self->field_02dc = 1;
        return 0;
    }

    if (state == 2)
    {
        // Original tail-jumps to the apply routine; its eax is forwarded unchanged.
        return CMenuControls_ApplySelectedConfiguration(self);
    }

    if (state != 4)
    {
        return static_cast<int>(state - 4);
    }

    int globalState = GetGlobalState();
    uint32_t schedulerProxy = static_cast<uint32_t>(
        SchedulerBase_GetSchedulerProxy(reinterpret_cast<void*>(globalState), nullptr));

    if (schedulerProxy - 1u == 0)
    {
        int refreshResult = MenuControls_RefreshSelectionAndAddItem(self, nullptr);
        self->field_02dc = 0xFFFFFFFFu;
        return refreshResult;
    }

    uint32_t proxyMinusTwo = schedulerProxy - 2u;
    if (proxyMinusTwo == 0)
    {
        self->field_02dc = 0xFFFFFFFFu;
        return 0;
    }

    return static_cast<int>(proxyMinusTwo);
}

HOOK(0x00542F10, CMenuControls_StepStateMachine, "auto");
