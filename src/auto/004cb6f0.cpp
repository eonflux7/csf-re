#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiSearchOrders_InitializeSearchPoint, 0x004A9DA0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, AiDistracted_GrowPointList, 0x004ADDB0,
         (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AiSearchOrders_InitializeSweepTiming, 0x004CB490,
         (void* self, void* edx, int a2, float a3, float a4, int* a5));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0,
         (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_DegreesToRadians, 0x00835440);

static int __fastcall TFncRastrear_InitializeSweep(
    TFncRastrear* self, void* /*edx*/, TAiState* state)
{
    // The packet also identifies this call target as a vtable reference.
    (void)CSF_VTBL(0x004CB490);

    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* linkedObject = reinterpret_cast<void*>(
        linkedValue ? linkedValue - 4 : 0);

    uint32_t* position = GetObjectPositionVector(
        CSF_AT(void*, linkedObject, 0x2C), nullptr);
    CSF_AT(uint32_t, self, 0x44) = position[0];
    CSF_AT(uint32_t, self, 0x48) = position[1];
    CSF_AT(uint32_t, self, 0x4C) = position[2];

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedObject = reinterpret_cast<void*>(
        linkedValue ? linkedValue - 4 : 0);

    void* actor = CSF_AT(void*, linkedObject, 0x2C);
    void* orientationProvider =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, actor, 0)[0x28 / 4])(actor, nullptr);
    void* orientation =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, orientationProvider, 0)[0x18 / 4])(
                orientationProvider, nullptr);
    double heading =
        reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, orientation, 0)[0x28 / 4])(
                orientation, nullptr);

    CSF_AT(float, self, 0x50) = heading * g_DegreesToRadians;
    CSF_AT(uint32_t, self, 0x28) = 0;

    alignas(8) uint8_t pointBytes[0x18];
    AiSearchOrders_InitializeSearchPoint(
        pointBytes, nullptr,
        reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(self) + 0x44));

    if (CSF_AT(uint32_t, self, 0x28) >=
        CSF_AT(uint32_t, self, 0x24))
    {
        AiDistracted_GrowPointList(
            reinterpret_cast<uint8_t*>(self) + 0x20, nullptr,
            CSF_AT(int, self, 0x2C));
    }

    uint32_t pointIndex = CSF_AT(uint32_t, self, 0x28);
    uint32_t pointStorage = CSF_AT(uint32_t, self, 0x20);
    CSF_AT(uint32_t, self, 0x28) = pointIndex + 1;
    void* destination = reinterpret_cast<void*>(
        pointStorage + 20 * pointIndex);

    CSF_AT(uint32_t, destination, 0x00) = CSF_AT(uint32_t, pointBytes, 0x00);
    CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, pointBytes, 0x04);
    CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, pointBytes, 0x08);
    CSF_AT(uint32_t, destination, 0x0C) = CSF_AT(uint32_t, pointBytes, 0x0C);
    CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, pointBytes, 0x10);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    linkedObject = reinterpret_cast<void*>(
        linkedValue ? linkedValue - 4 : 0);

    float sweepDuration = CSF_AT(
        float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xAB0);
    CSF_AT(int, self, 0x18) = 0;
    AiSearchOrders_InitializeSweepTiming(
        linkedObject, nullptr, 2, 0.0f, sweepDuration,
        reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x10));

    return CoverAttack_SetState_4DC150(
        state, nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4));
}

HOOK(0x004CB6F0, TFncRastrear_InitializeSweep, "auto");
