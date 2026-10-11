#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_GetProviderPosition, 0x004F4EB0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float* __fastcall C_0082dfd0_ComputeProviderOffset(
    C_0082dfd0* self, void* /*edx*/, float* output, int /*unused*/)
{
    const int globalValue = GetGlobalValue_4FBDE0();
    const int firstElement =
        GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider =
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    const float* position = reinterpret_cast<float*>(
        MpSpawnPoints_GetProviderPosition(provider, nullptr));

    // The direction and saved position share the original 0x18-byte buffer.
    float vectors[6];
    vectors[4] = position[1];
    vectors[3] = position[0];
    vectors[5] = position[2];

    const float verticalZero = vectors[4] * g_fZero;
    vectors[0] = vectors[5] - verticalZero;
    vectors[1] = vectors[3] * g_fZero - vectors[5] * g_fZero;
    vectors[2] = verticalZero - vectors[3];
    NormalizeVector3(vectors, nullptr);

    const float forwardDistance = self->field_00f0 - self->field_00f4;
    const float lateralDistance = self->field_00fc - self->field_00f8;

    output[0] = vectors[3] * forwardDistance;
    output[1] = vectors[4] * forwardDistance;
    output[2] = vectors[5] * forwardDistance;

    const float lateralX = vectors[0] * lateralDistance;
    const float lateralY = vectors[1] * lateralDistance;
    const float lateralZ = vectors[2] * lateralDistance;

    output[0] = lateralX + output[0];
    output[1] = lateralY + output[1];
    output[2] = lateralZ + output[2];
    return output;
}

HOOK(0x00411930, C_0082dfd0_ComputeProviderOffset, "auto");
