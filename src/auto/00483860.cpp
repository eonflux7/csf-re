#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, InnerObject_IsIdle, 0x004950F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_IsSubObjectBusy, 0x00478280, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_ClearSubObjectQueue, 0x004782E0, (void* self, void* edx, int a4));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140, (void* self, void* edx));

// Resets the inner object forwarding and weapon sub-state, then dispatches the
// state-refresh virtual (slot 71) with 54 or 8 depending on the flag at +0xA8.
static int __fastcall CharActionFsm_ResetInnerObjectsAndRefreshState(void* self, void* /*edx*/)
{
    void* innerObj = CSF_AT(void*, self, 0x08);
    if (!InnerObject_IsIdle(innerObj, nullptr))
    {
        // Original pushes 1 before this call; the bind signature above does not carry it.
        C_00495140_ForwardInnerObjectVirtualCall(innerObj, nullptr);
    }

    if (CSF_AT(int, self, 0x24) != -1)
    {
        void* weaponState = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[54])(self, nullptr);
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, weaponState, 0)[12])(weaponState, nullptr, 0);
    }

    void* subObject = CSF_AT(void*, self, 0xA4);
    ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[55])(subObject, nullptr, 0);

    if (CharActionFsm_IsSubObjectBusy(CSF_AT(void*, self, 0xA4), nullptr) != 0)
    {
        CharActionFsm_ClearSubObjectQueue(CSF_AT(void*, self, 0xA4), nullptr, 0);
    }

    int refreshArg = (CSF_AT(uint8_t, self, 0xA8) != 0) ? 54 : 8;
    return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[71])(self, nullptr, refreshArg);
}

HOOK(0x00483860, CharActionFsm_ResetInnerObjectsAndRefreshState, "auto");
