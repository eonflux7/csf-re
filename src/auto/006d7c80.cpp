#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

static std::uint32_t __fastcall S_006d7c80_UpdateParticlePositions(
    S_006d7c80* self, void* /*edx*/, void* source)
{
    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t directionBytes[0x8];
    alignas(8) std::uint8_t matrixResultBytes[0x8];

    void* sourceObject =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x14 / 4]))(source, nullptr);

    void* sourcePosition =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, sourceObject, 0)[0x10 / 4]))(sourceObject, nullptr);

    sourceObject =
        (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x14 / 4]))(source, nullptr);

    void* matrixResult =
        (reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, sourceObject, 0)[0x8C / 4]))(
                sourceObject, nullptr, matrixResultBytes);

    auto* matrixDirection =
        static_cast<std::uint8_t*>(CSF_AT(void*, matrixResult, 0)) + 0x20;

    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    const float z = CSF_AT(float, sourcePosition, 0x8);
    const float y = CSF_AT(float, sourcePosition, 0x4);
    CSF_AT(std::uint32_t, positionBytes, 0) =
        CSF_AT(std::uint32_t, sourcePosition, 0);

    std::uint32_t count = self->field_00c0;
    CSF_AT(float, positionBytes, 0x4) = y;
    CSF_AT(float, positionBytes, 0x8) = z;

    for (std::uint32_t index = 0; index < count; ++index)
    {
        auto* particle = CSF_AT(
            std::uint8_t*, reinterpret_cast<void*>(self->field_00b8), index * 4);
        void* particleCollection = particle + 0x20;
        const std::uint32_t requestedIndex = self->field_00ec;

        const std::uint32_t availableCount =
            (reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, particleCollection, 0)[0x14 / 4]))(
                    particleCollection, nullptr);

        particle = CSF_AT(
            std::uint8_t*, reinterpret_cast<void*>(self->field_00b8), index * 4);
        particleCollection = particle + 0x20;

        void* selectedParticle;
        if (requestedIndex >= availableCount)
        {
            selectedParticle =
                (reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, particleCollection, 0)[0xC / 4]))(
                        particleCollection, nullptr, positionBytes);
        }
        else
        {
            selectedParticle =
                (reinterpret_cast<void* (__fastcall*)(
                    void*, void*, std::uint32_t)>(
                        CSF_AT(void**, particleCollection, 0)[0x18 / 4]))(
                            particleCollection, nullptr, requestedIndex);
        }

        if (!self->field_00ec)
        {
            particle = CSF_AT(
                std::uint8_t*, reinterpret_cast<void*>(self->field_00b8), index * 4);
            particleCollection = particle + 0x20;

            auto* emitter =
                (reinterpret_cast<std::uint8_t* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, particleCollection, 0)[0x8 / 4]))(
                        particleCollection, nullptr);

            CSF_AT(std::uint32_t, directionBytes, 0x4) =
                CSF_AT(std::uint32_t, matrixDirection, 0x8);
            CSF_AT(std::uint32_t, directionBytes, 0) =
                CSF_AT(std::uint32_t, matrixDirection, 0);

            void* emitterSettings = emitter + 0x20;
            (reinterpret_cast<void (__fastcall*)(void*, void*, void*, void*, int)>(
                CSF_AT(void**, emitterSettings, 0)[0x14 / 4]))(
                    emitterSettings, nullptr, positionBytes, directionBytes, 1);
        }

        (reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, selectedParticle, 0)[0x38 / 4]))(
                selectedParticle, nullptr, positionBytes);

        count = self->field_00c0;
    }

    return count;
}

HOOK(0x006D7C80, S_006d7c80_UpdateParticlePositions, "auto");
