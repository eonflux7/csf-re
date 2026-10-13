#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ApplyTargetPointRequest, 0x0040FC50, (void* self, void* , void* request));
CSF_FUNC(char, __fastcall, S_0048c9f0_GetPendingTargetMask, 0x0048CCA0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_0048c9f0_ProcessTargetChannel, 0x0048CD50, (void* self, void* edx, int _58, float a3));
CSF_FUNC(void, __fastcall, S_0048c9f0_UpdateState, 0x0048D050, (void* self, void* edx));

static void __fastcall S_0048c9f0_UpdateTargetPoints(
    S_0048c9f0* self, void* /*edx*/, float elapsedTime)
{
    S_0048c9f0_UpdateState(self, nullptr);

    if ((self->field_0039 & 3) != 0)
    {
        const char targetMask = S_0048c9f0_GetPendingTargetMask(self, nullptr);
        if (targetMask != 0)
        {
            if ((targetMask & 1) != 0)
                S_0048c9f0_ProcessTargetChannel(self, nullptr, 1, elapsedTime);
            if ((targetMask & 2) != 0)
                S_0048c9f0_ProcessTargetChannel(self, nullptr, 2, elapsedTime);
            if ((targetMask & 4) != 0)
                S_0048c9f0_ProcessTargetChannel(self, nullptr, 4, elapsedTime);
        }
        return;
    }

    alignas(8) uint8_t requestBytes[0x14]{};

    if (static_cast<int16_t>(self->field_0022) > 0)
    {
        requestBytes[0] = 0;
        CSF_AT(int, requestBytes, 0x4) = 0;
        void* owner = CSF_AT(void*, self, 0x4);
        ApplyTargetPointRequest(
            CSF_AT(void*, owner, 0x4C), nullptr, requestBytes);
    }

    if (static_cast<int16_t>(self->field_0024) > 0)
    {
        requestBytes[0] = 0;
        CSF_AT(int, requestBytes, 0x4) = 1;
        void* owner = CSF_AT(void*, self, 0x4);
        ApplyTargetPointRequest(
            CSF_AT(void*, owner, 0x4C), nullptr, requestBytes);
    }
}

HOOK(0x0048D0F0, S_0048c9f0_UpdateTargetPoints, "auto");
