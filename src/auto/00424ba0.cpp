#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TCharTask_ForwardUpdateToChild, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_0082faf8_ApplyFloatStep, 0x00424130, (void* self, void* edx, float a2));
CSF_FUNC(uint32_t*, __fastcall, C_0082faf8_ComputeFloatStep, 0x00424940, (void* self, void* edx, float a5));
CSF_FUNC(void, __fastcall, C_0082faf8_Sub424360, 0x00424360, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082faf8_Sub4241F0, 0x004241F0, (void* self, void* edx));

// Forwards the update to the base task, then, if field_0020 is set, applies the
// float step and runs the finish sequence unless field_0074 is set.
static void __fastcall C_0082faf8_UpdateWithChildTask(C_0082faf8* self, void* /*edx*/, int a2)
{
    float stepValue;
    std::memcpy(&stepValue, &a2, sizeof(stepValue));

    TCharTask_ForwardUpdateToChild(self, nullptr, a2);

    if (self->field_0020)
    {
        C_0082faf8_ApplyFloatStep(self, nullptr, stepValue);
        // The original's last write to ecx before this call is `mov ecx, esi` (this).
        C_0082faf8_ComputeFloatStep(self, nullptr, stepValue);

        if (!self->field_0074)
        {
            C_0082faf8_Sub424360(self, nullptr);
            C_0082faf8_Sub4241F0(self, nullptr);
        }
    }
}

HOOK(0x424BA0, C_0082faf8_UpdateWithChildTask, "auto");
