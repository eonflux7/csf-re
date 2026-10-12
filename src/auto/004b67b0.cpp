#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453660, 0x00453660, (CGuiControl* self, void* , S_0041e5f0* a2, float* a3, int* a4, float a5));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// This address is also classified as a vtable reference by the packet.
static const auto kArrayCapacityReference = CSF_VTBL(0x0041E5F0);

static bool __stdcall AiDistracted_CheckDistractionTarget(int target)
{
    alignas(8) uint8_t filterBytes[0x10];
    alignas(8) uint8_t excludedBytes[0x24];
    alignas(8) uint8_t sphereBytes[0x10];
    alignas(8) uint8_t resultsBytes[0x10];

    int* filter = reinterpret_cast<int*>(filterBytes);
    S_0041e5f0& excluded =
        *reinterpret_cast<S_0041e5f0*>(excludedBytes);
    float* sphere = reinterpret_cast<float*>(sphereBytes);
    S_0041e5f0& results =
        *reinterpret_cast<S_0041e5f0*>(resultsBytes);

    int linkedValue =
        GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(target), nullptr);
    void* linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    float* position = GetObjectPositionVector(
        CSF_AT(void*, linkedObject, 0x2C), nullptr);
    sphere[0] = position[0];
    sphere[1] = position[1];
    sphere[2] = position[2];

    results.field_000c = 10;
    excluded.field_000c = 10;
    sphere[3] = 200.0f;

    results.field_0000 = 0;
    results.field_0004 = 0;
    results.field_0008 = 0;
    excluded.field_0000 = 0;
    excluded.field_0004 = 0;
    excluded.field_0008 = 0;
    excluded.field_0010 = 0;
    excluded.field_0014 = 0;
    excluded.field_0018 = 0;

    filter[1] = -1;
    filter[2] = -1;
    filter[3] = -1;
    excluded.field_001c = static_cast<uint32_t>(-1);
    excluded.field_0020 = 0;
    filter[0] = 274;

    linkedValue =
        GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(target), nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;
    uint32_t excludedObject = CSF_AT(uint32_t, linkedObject, 0x2C);

    if (excluded.field_0008 >= excluded.field_0004)
    {
        S_0041e5f0_GrowArrayCapacity(
            &excluded, nullptr, static_cast<int>(excluded.field_000c));
    }

    uint32_t* entry =
        reinterpret_cast<uint32_t*>(excluded.field_0000) + excluded.field_0008;
    ++excluded.field_0008;
    *entry = excludedObject;

    CGuiControl_ProcessDisplayFlags_453660(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene),
        nullptr, &results, sphere, filter, 0.0f);

    bool noDistractionTarget = results.field_0008 == 0;

    if (excluded.field_0000)
    {
        Mem_Free(reinterpret_cast<void*>(excluded.field_0000));
        excluded.field_0000 = 0;
    }
    if (results.field_0000)
        Mem_Free(reinterpret_cast<void*>(results.field_0000));

    return noDistractionTarget;
}

HOOK(0x004B67B0, AiDistracted_CheckDistractionTarget, "auto");
