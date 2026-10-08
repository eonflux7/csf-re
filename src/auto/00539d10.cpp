#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game-state accessors and helpers called by the original.
CSF_FUNC(uintptr_t, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(void, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GlobalState_SetAuxFlag, 0x0050DC80, (void* self, void* edx, char flag));
CSF_FUNC(uintptr_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
// Original passes the GetGlobal8afc78 result in ecx and a2 on the stack (ret 4).
CSF_FUNC(int, __fastcall, MenuResult_Finalize, 0x0042DF80, (void* ecxValue, void* edxUnused, int a1));

static void* CurrentGlobalState()
{
    return reinterpret_cast<void*>(GetGlobalState());
}

static int __fastcall CMenuPause_CloseAndReturnResult(CMenuPause* self, void* /*edx*/, int a2)
{
    MenuSystem_ToggleChildFlag(CurrentGlobalState(), nullptr);

    if (CSF_AT(uint8_t, CurrentGlobalState(), 0x89))
    {
        GlobalState_SetAuxFlag(CurrentGlobalState(), nullptr, static_cast<char>(1));
    }

    uint32_t stateChildAddr = CSF_AT(uint32_t, CurrentGlobalState(), 0x10);
    if (stateChildAddr)
    {
        void* stateChild = reinterpret_cast<void*>(stateChildAddr);
        void** stateChildVtbl = CSF_AT(void**, stateChild, 0);
        // vtable slot at byte offset 0x24
        ((void(__fastcall*)(void*, void*))stateChildVtbl[0x24 / 4])(stateChild, nullptr);
    }

    void* pauseChild = reinterpret_cast<void*>(self->field_0010);
    void** pauseChildVtbl = CSF_AT(void**, pauseChild, 0);
    // vtable slot at byte offset 0x60, args: (self, 1)
    ((void(__fastcall*)(void*, void*, void*, int))pauseChildVtbl[0x60 / 4])(pauseChild, nullptr, self, 1);

    uintptr_t globalStateResult = GetGlobal8afc78();
    return MenuResult_Finalize(reinterpret_cast<void*>(globalStateResult), nullptr, a2);
}

HOOK(0x00539D10, CMenuPause_CloseAndReturnResult, "auto");
