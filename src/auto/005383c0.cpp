#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x0050D190: returns the global state pointer (cdecl, no args).
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
// 0x0050E130: resets menu resources on the global state (thiscall, no stack args).
CSF_FUNC(int, __fastcall, SceneLoader_ResetMenuResources, 0x0050E130, (void* self, void* edx));
// 0x00538240: applies the refresh step to the scene instancing object (thiscall, one stack arg).
CSF_FUNC(void, __fastcall, SceneInstancing_ApplyRefreshStep, 0x00538240, (void* self, void* edx, void* a2));

// Float 0.1f (bit pattern 0x3DCCCCCD) passed as the stack argument to the callee.
static const uint32_t kRefreshStepBits = 0x3DCCCCCDu;

static void __fastcall SceneInstancing_ResetState(void* self, void* /*edx*/, int /*unusedArg*/)
{
    uint8_t* instance = static_cast<uint8_t*>(self);

    if (CSF_AT(uint8_t, instance, 0x108))
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        SceneLoader_ResetMenuResources(globalState, nullptr);

        CSF_AT(int, instance, 0x130) = 2;
        CSF_AT(int, instance, 0x100) = 0;
        CSF_AT(int, instance, 0x104) = 0;
        CSF_AT(uint8_t, instance, 0x108) = 0;
        CSF_AT(uint8_t, instance, 0x109) = 0;
        CSF_AT(int, instance, 0x10C) = 0;
        CSF_AT(uint8_t, instance, 0x110) = 1;
        CSF_AT(uint8_t, instance, 0x111) = 0;
        CSF_AT(uint8_t, instance, 0x112) = 0;

        SceneInstancing_ApplyRefreshStep(
            instance, nullptr,
            reinterpret_cast<void*>(static_cast<uintptr_t>(kRefreshStepBits)));
    }
}

HOOK(0x005383C0, SceneInstancing_ResetState, "auto");
