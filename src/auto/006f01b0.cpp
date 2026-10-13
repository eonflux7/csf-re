#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpPTankAtomicGetDataFormat, 0x005821B0, (int a1));
CSF_FUNC(int, __cdecl, RpPTankAtomicCreate, 0x00582400, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(void*, __cdecl, RwFrameCreate, 0x006235D0, ());
CSF_FUNC(void*, __fastcall, GfxPtank_GetPlacementState, 0x006F3FE0, (void* self, void* , void* outState));

static int __fastcall GfxParticlePtank_CreateAtomic(void* self, void* /*edx*/)
{
    alignas(8) uint8_t placementState[0x28];
    GfxPtank_GetPlacementState(
        static_cast<uint8_t*>(self) + 0x24, nullptr, placementState);

    void* placement = CSF_AT(void*, placementState, 0x10);
    const int creationFlags = CSF_AT(int, placement, 0x04);
    const int dataFormatAddress =
        RpPTankAtomicGetDataFormat(CSF_AT(int, placement, 0x18));
    const int dataFlags = CSF_AT(
        int, reinterpret_cast<void*>(static_cast<uintptr_t>(dataFormatAddress)), 0x08);

    placement = CSF_AT(void*, placementState, 0x10);
    const int atomic =
        RpPTankAtomicCreate(CSF_AT(int, placement, 0x0C), dataFlags, creationFlags);

    void* frame = RwFrameCreate();
    const uint32_t frameFlags = CSF_AT(uint32_t, frame, 0x1C);
    CSF_AT(float, frame, 0x38) = 1.0f;
    CSF_AT(float, frame, 0x24) = 1.0f;
    CSF_AT(float, frame, 0x10) = 1.0f;
    CSF_AT(float, frame, 0x20) = 0.0f;
    CSF_AT(float, frame, 0x18) = 0.0f;
    CSF_AT(float, frame, 0x14) = 0.0f;
    CSF_AT(float, frame, 0x34) = 0.0f;
    CSF_AT(float, frame, 0x30) = 0.0f;
    CSF_AT(float, frame, 0x28) = 0.0f;
    CSF_AT(float, frame, 0x48) = 0.0f;
    CSF_AT(float, frame, 0x44) = 0.0f;
    CSF_AT(float, frame, 0x40) = 0.0f;
    CSF_AT(uint32_t, frame, 0x1C) = frameFlags | 0x20003;
    RpAtomicSetFrame(atomic, static_cast<int>(reinterpret_cast<uintptr_t>(frame)));

    auto* atomics =
        reinterpret_cast<S_0041e5f0*>(static_cast<uint8_t*>(self) + 0xBC);
    if (CSF_AT(uint32_t, self, 0xC4) >= CSF_AT(uint32_t, self, 0xC0))
        S_0041e5f0_GrowArrayCapacity(atomics, nullptr, atomics->field_000c);

    // Address annotation supplied for the direct GrowArrayCapacity call.
    (void)CSF_VTBL(0x0041E5F0);

    const uint32_t index = atomics->field_0008;
    auto* storage = reinterpret_cast<int*>(
        static_cast<uintptr_t>(atomics->field_0000));
    int* slot = storage + index;
    atomics->field_0008 = index + 1;
    *slot = atomic;
    return atomic;
}

HOOK(0x006F01B0, GfxParticlePtank_CreateAtomic, "auto");
