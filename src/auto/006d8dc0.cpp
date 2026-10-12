#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void, __stdcall, GfxPTank_WrapParticlePositions, 0x006D83F0, (void* emitter));
CSF_FUNC(void*, __fastcall, GfxPtank_GetPlacementState, 0x006F3FE0, (void* self, void* edx, void* a2));

CSF_GLOBAL(float, g_WeaponPlacementTraceDepth, 0x0082E11C);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static std::uint32_t __fastcall S_006d8dc0_UpdateParticlePlacement(
    S_006d8dc0* self, void* /*edx*/, void* source)
{
    alignas(8) std::uint8_t placementBytes[0x34];
    alignas(8) std::uint8_t orientationBytes[0x10];
    alignas(8) std::uint8_t matrixBytes[0x14];

    void* transform =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);
    float* position =
        ((float* (__fastcall*)(void*, void*))
            CSF_AT(void**, transform, 0)[0x10 / 4])(transform, nullptr);

    transform =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);
    void* matrixResult =
        ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, transform, 0)[0x8C / 4])(
                transform, nullptr, matrixBytes);
    auto* orientationSource =
        reinterpret_cast<std::uint8_t*>(CSF_AT(void*, matrixResult, 0)) + 0x20;

    if (CSF_AT(std::uint8_t, matrixBytes, 0x04))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    std::uint32_t placementFlags =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(source));
    std::uint32_t count = self->field_00b0;
    std::uint32_t index = 0;

    while (index < count)
    {
        auto* controller = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<void**>(self->field_00a8)[index]) + 0x20;
        const std::uint32_t requestedIndex = self->field_00e8;
        const std::uint32_t availableCount =
            ((std::uint32_t (__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[0x14 / 4])(controller, nullptr);

        controller = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<void**>(self->field_00a8)[index]) + 0x20;

        void* particle;
        if (requestedIndex >= availableCount)
        {
            particle =
                ((void* (__fastcall*)(void*, void*, float*))
                    CSF_AT(void**, controller, 0)[0x0C / 4])(
                        controller, nullptr, position);
        }
        else
        {
            particle =
                ((void* (__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, controller, 0)[0x18 / 4])(
                        controller, nullptr, requestedIndex);
        }

        if (!self->field_00e8)
        {
            controller = reinterpret_cast<std::uint8_t*>(
                reinterpret_cast<void**>(self->field_00a8)[index]) + 0x20;
            void* emitter =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, controller, 0)[0x08 / 4])(
                        controller, nullptr);
            float* oldPosition =
                ((float* (__fastcall*)(void*, void*))
                    CSF_AT(void**, emitter, 0)[0x48 / 4])(emitter, nullptr);

            float& deltaX = CSF_AT(float, placementBytes, 0x28);
            float& deltaY = CSF_AT(float, placementBytes, 0x2C);
            float& deltaZ = CSF_AT(float, placementBytes, 0x30);
            deltaX = position[0] - oldPosition[0];
            deltaY = position[1] - oldPosition[1];
            deltaZ = position[2] - oldPosition[2];

            const bool distant =
                deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX >
                g_WeaponPlacementTraceDepth;
            placementFlags = (placementFlags & 0xFFFFFF00u) |
                             static_cast<std::uint32_t>(distant);

            GfxPtank_GetPlacementState(emitter, nullptr, placementBytes);

            void* placementObject = CSF_AT(void*, placementBytes, 0x1C);
            void* placementData = CSF_AT(void*, placementObject, 0);
            const std::uint32_t placementMode =
                CSF_AT(std::uint32_t, placementData, 0xD4);
            const std::uint32_t orientationSecond =
                CSF_AT(std::uint32_t, orientationSource, 0x08);
            const std::uint32_t orientationFirst =
                CSF_AT(std::uint32_t, orientationSource, 0);

            std::uint8_t* orientation;
            if (placementMode)
            {
                CSF_AT(std::uint32_t, orientationBytes, 0x04) = orientationSecond;
                CSF_AT(std::uint32_t, orientationBytes, 0) = orientationFirst;
                orientation = orientationBytes;
            }
            else
            {
                CSF_AT(std::uint32_t, orientationBytes, 0x0C) = orientationSecond;
                CSF_AT(std::uint32_t, orientationBytes, 0x08) = orientationFirst;
                orientation = orientationBytes + 0x08;
            }

            auto* placementController =
                reinterpret_cast<std::uint8_t*>(emitter) + 0x20;
            ((void (__fastcall*)(void*, void*, float*, void*, std::uint32_t))
                CSF_AT(void**, placementController, 0)[0x14 / 4])(
                    placementController, nullptr, position, orientation,
                    placementFlags);

            if ((deltaX != g_ZeroVectorX ||
                 deltaY != g_ZeroVectorY ||
                 deltaZ != g_ZeroVectorZ) &&
                !(placementFlags & 0xFF))
            {
                GfxPTank_WrapParticlePositions(emitter);
            }
        }

        ((void (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, particle, 0)[0x38 / 4])(particle, nullptr, position);

        count = self->field_00b0;
        ++index;
    }

    return count;
}

HOOK(0x006D8DC0, S_006d8dc0_UpdateParticlePlacement, "auto");
