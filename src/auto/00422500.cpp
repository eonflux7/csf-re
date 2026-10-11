#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(void, __fastcall, RefreshOwnerCachedState, 0x004220F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ApplyOwnerAnimationStep, 0x00421050, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SetTaskIdleFlag, 0x004DB1D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SyncTaskAnchorOffset, 0x004DB440, (void* self, void* edx, int a2));

static uint8_t* AddressToPointer(uint32_t address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

// Stack args a2/a3 are popped by the callee (ret 8).
static void __fastcall TCharTask_UpdateMovementOrIdle(TCharTask* self, void* /*edx*/, int a2, int a3)
{
    RefreshOwnerCachedState(AddressToPointer(CSF_AT(uint32_t, self, 0x08)), nullptr);

    uint8_t* owner = AddressToPointer(CSF_AT(uint32_t, self, 0x08));
    if (CSF_AT(float, owner, 0x200) <= g_fZero)
    {
        uint8_t* controller = AddressToPointer(CSF_AT(uint32_t, AddressToPointer(CSF_AT(uint32_t, owner, 0x14)), 0x48));
        int stopResult = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, controller, 0)[0x64 / 4])(controller, nullptr);
        if (stopResult != -1)
        {
            owner = AddressToPointer(CSF_AT(uint32_t, self, 0x08));
            uint8_t* controllerAgain = AddressToPointer(CSF_AT(uint32_t, AddressToPointer(CSF_AT(uint32_t, owner, 0x14)), 0x48));
            reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, controllerAgain, 0)[0x68 / 4])(controllerAgain, nullptr);
        }
        SetTaskIdleFlag(self, nullptr, 0);
    }
    else
    {
        ApplyOwnerAnimationStep(AddressToPointer(CSF_AT(uint32_t, self, 0x0C)), nullptr);
        SyncTaskAnchorOffset(self, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 60));
    }
}

HOOK(0x00422500, TCharTask_UpdateMovementOrIdle, "auto");
