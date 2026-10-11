#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0083e160_Initialize, 0x004E1200, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D860, 0x0050D860, (void* self, void*));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __fastcall C_0083e7d8_InitializeTransition(
    C_0083e7d8* self, void* /*edx*/,
    int parentAddress, int value1, int value2, int value3, int value4)
{
    C_0083e160_Initialize(self, nullptr, parentAddress, 1);

    double transitionScale = g_TransitionOne;
    self->vtbl = CSF_VTBL(0x0083E7D8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x0083E7BC));
    self->field_002c = 0;
    self->field_0030 = 0.0f;
    self->field_0034 = 0;

    if (parentAddress &&
        CSF_AT(int, reinterpret_cast<void*>(parentAddress), 0x78) == 1)
    {
        int globalState = GetGlobalState_50D190();
        transitionScale = MenuSystem_GetTransitionScale_50D860(
            reinterpret_cast<void*>(globalState), nullptr);
    }

    self->field_0018 = static_cast<uint32_t>(
        static_cast<int64_t>(static_cast<double>(value1) * transitionScale));
    self->field_001c = static_cast<uint32_t>(
        static_cast<int64_t>(static_cast<double>(value2) * transitionScale));
    self->field_0020 = static_cast<uint32_t>(
        static_cast<int64_t>(static_cast<double>(value3) * transitionScale));
    self->field_0024 = static_cast<uint32_t>(
        static_cast<int64_t>(static_cast<double>(value4) * transitionScale));

    self->field_0028 = 0;
    self->field_0038 = 0;
    self->field_0044 = 0.0f;
    self->field_003c = 0.0f;
    self->field_0040 = 0.0f;
    self->field_004c = 0;
    self->field_0048 = 0;
    self->field_0050 = 0.0f;
    self->field_0054 = 0;

    return reinterpret_cast<int>(self);
}

HOOK(0x004E4630, C_0083e7d8_InitializeTransition, "auto");
