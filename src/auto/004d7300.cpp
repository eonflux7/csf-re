#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_SetCoverTransition, 0x004D1100, (void* self, void* edx, int a2, int a3));

// Resets the cover transition on the owner's sub-object at +0x330 using the
// global's transition value at +0xAEC, then dispatches virtual slot 35 (+0x8C)
// on the object stored at owner+0x14 with arguments (0, 0, -1, 0).
static int __fastcall TAutonomo_ResetCoverAndRunAction(void* self, void* /*edx*/)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);

    uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    uint32_t coverTransitionValue = CSF_AT(uint32_t, globalBase, 0x0AEC);

    AiCoverAttackTasks_SetCoverTransition(owner + 0x330, nullptr, 0, static_cast<int>(coverTransitionValue));

    void* actionObject = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    using DispatchFn = int(__fastcall*)(void*, void*, int, int, int, int);
    DispatchFn dispatch = reinterpret_cast<DispatchFn>(CSF_AT(void**, actionObject, 0)[0x8C / 4]);
    return dispatch(actionObject, nullptr, 0, 0, -1, 0);
}

HOOK(0x004D7300, TAutonomo_ResetCoverAndRunAction, "auto");
