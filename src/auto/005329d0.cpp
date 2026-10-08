#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, NoOpCallback_506AC0, 0x506AC0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, (int argBits));
CSF_FUNC(void, __fastcall, ApplyGlobalValueDelta, 0x5231D0, (void* self, void* edx, int deltaBits));
CSF_FUNC(void*, __fastcall, SetRotationWrapFlag, 0x5322A0, (CMenuMpServerInfo* self, void* edx, int negativeFlag));

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_fRotationWrapStep, 0x82E1E4);

// Decrements the float timer at +0x4C by the float passed in (raw bits on the stack),
// updates the negative-flag via SetRotationWrapFlag, and wraps the timer by adding
// the step constant when it goes below zero. Does nothing when the byte at +0x1F4 is set.
static void __fastcall CMenuMpServerInfo_DecrementRotationTimer(CMenuMpServerInfo* self, void* /*edx*/, int deltaBits)
{
    NoOpCallback_506AC0(self, nullptr);

    if (self->field_01f4 != 0)
        return;

    int globalValue = GetGlobalValue(deltaBits);
    ApplyGlobalValueDelta(
        reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(globalValue))),
        nullptr,
        deltaBits);

    float deltaSeconds;
    std::memcpy(&deltaSeconds, &deltaBits, sizeof(float));

    float updatedTimer = self->field_004c - deltaSeconds;
    self->field_004c = updatedTimer;

    SetRotationWrapFlag(self, nullptr, (updatedTimer < g_fZero) ? 1 : 0);

    if (self->field_004c < g_fZero)
        self->field_004c = self->field_004c + g_fRotationWrapStep;
}

HOOK(0x005329D0, CMenuMpServerInfo_DecrementRotationTimer, "auto");
