#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __stdcall, Motion_AdvanceQueuedSegment, 0x0040A660, (void* state, float elapsedTime));
CSF_FUNC(char, __fastcall, S_0040a7a0_ApplyTransform, 0x0040A7A0, (S_0040a7a0* self, void* , int operation, char local, int valueBits));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// The packet labels this address as a vtable reference, but the
// instructions use it only as the direct ApplyTransform call target.
static const auto g_ApplyTransformVtableReference = CSF_VTBL(0x0040A7A0);

static void __fastcall S_0040a7a0_UpdateChildTransforms(
    S_0040a7a0* self, void* /*edx*/, float elapsedTime)
{
    int operation = 0;
    for (int offset = 0; offset < 224; offset += 28)
    {
        float value = static_cast<float>(Motion_AdvanceQueuedSegment(
            reinterpret_cast<uint8_t*>(self->field_0010) + offset,
            elapsedTime));

        if (value != g_fZero)
        {
            int valueBits;
            std::memcpy(&valueBits, &value, sizeof(valueBits));
            S_0040a7a0_ApplyTransform(
                self, nullptr, operation, 1, valueBits);
        }
        ++operation;
    }

    operation = 0;
    for (int offset = 0; offset < 224; offset += 28)
    {
        float value = static_cast<float>(Motion_AdvanceQueuedSegment(
            reinterpret_cast<uint8_t*>(self->field_0020) + offset,
            elapsedTime));

        if (value != g_fZero)
        {
            int valueBits;
            std::memcpy(&valueBits, &value, sizeof(valueBits));
            S_0040a7a0_ApplyTransform(
                self, nullptr, operation, 0, valueBits);
        }
        ++operation;
    }
}

HOOK(0x0040AFA0, S_0040a7a0_UpdateChildTransforms, "auto");
