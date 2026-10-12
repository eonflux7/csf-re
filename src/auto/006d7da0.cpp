#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(double, __fastcall, S_00526d40_NormalizeCopyAndReportY, 0x006AB9E0, (S_00526d40* self, void*));
CSF_FUNC(void*, __fastcall, GfxPtank_GetPlacementState, 0x006F3FE0, (void* self, void* edx, void* a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);

static std::uint32_t __fastcall GfxPtank_UpdateParticlePlacement(
    void* self, void* /*edx*/, void* source)
{
    void* sourceObject =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);
    void* position =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, sourceObject, 0)[0x10 / 4])(sourceObject, nullptr);

    sourceObject =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);

    alignas(8) std::uint8_t matrixBytes[0x20];
    void* matrixResult =
        ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, sourceObject, 0)[0x8C / 4])(
                sourceObject, nullptr, matrixBytes);
    auto* direction = reinterpret_cast<S_00526d40*>(
        static_cast<std::uint8_t*>(CSF_AT(void*, matrixResult, 0)) + 0x20);

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    const float directionY =
        static_cast<float>(S_00526d40_NormalizeCopyAndReportY(direction, nullptr));

    std::uint32_t count = CSF_AT(std::uint32_t, self, 0xD0);
    for (std::uint32_t index = 0; index < count; ++index)
    {
        void* collection = static_cast<std::uint8_t*>(
            CSF_AT(void**, self, 0xC8)[index]) + 0x20;
        const std::uint32_t selectedIndex = CSF_AT(std::uint32_t, self, 0xF0);
        const std::uint32_t available =
            ((std::uint32_t (__fastcall*)(void*, void*))
                CSF_AT(void**, collection, 0)[0x14 / 4])(collection, nullptr);

        collection = static_cast<std::uint8_t*>(
            CSF_AT(void**, self, 0xC8)[index]) + 0x20;

        void* particle;
        if (selectedIndex >= available)
        {
            particle =
                ((void* (__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, collection, 0)[0x0C / 4])(
                        collection, nullptr, position);
        }
        else
        {
            particle =
                ((void* (__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, collection, 0)[0x18 / 4])(
                        collection, nullptr, selectedIndex);
        }

        if (!CSF_AT(std::uint32_t, self, 0xF0))
        {
            collection = static_cast<std::uint8_t*>(
                CSF_AT(void**, self, 0xC8)[index]) + 0x20;
            void* placement =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, collection, 0)[0x08 / 4])(collection, nullptr);
            void* previousPosition =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, placement, 0)[0x48 / 4])(placement, nullptr);

            const float dx = CSF_AT(float, position, 0) -
                             CSF_AT(float, previousPosition, 0);
            const float dy = CSF_AT(float, position, 4) -
                             CSF_AT(float, previousPosition, 4);
            const float dz = CSF_AT(float, position, 8) -
                             CSF_AT(float, previousPosition, 8);
            const int distant =
                dx * dx + dy * dy + dz * dz > g_WeaponPlacementTraceDepth;

            alignas(8) std::uint8_t stateBytes[0x28];
            GfxPtank_GetPlacementState(placement, nullptr, stateBytes);

            const float magnitude = std::fabs(directionY);
            const double duration =
                ((double (__fastcall*)(void*, void*))
                    CSF_AT(void**, source, 0)[0x24 / 4])(source, nullptr);

            void* state = CSF_AT(void*, stateBytes, 0);
            if (duration * g_HalfDurationScale > magnitude)
                CSF_AT(std::uint32_t, state, 0x24) =
                    CSF_AT(std::uint32_t, state, 0x30);
            else
                CSF_AT(std::uint32_t, state, 0x24) = 0;

            alignas(8) std::uint8_t horizontalDirection[0x8];
            CSF_AT(std::uint32_t, horizontalDirection, 4) = direction[2].field_0000;
            CSF_AT(std::uint32_t, horizontalDirection, 0) = direction->field_0000;

            void* placementMember = static_cast<std::uint8_t*>(placement) + 0x20;
            ((void (__fastcall*)(void*, void*, void*, void*, int))
                CSF_AT(void**, placementMember, 0)[0x14 / 4])(
                    placementMember, nullptr, position, horizontalDirection, distant);
        }

        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, particle, 0)[0x38 / 4])(particle, nullptr, position);
        count = CSF_AT(std::uint32_t, self, 0xD0);
    }
    return count;
}

HOOK(0x006D7DA0, GfxPtank_UpdateParticlePlacement, "auto");
