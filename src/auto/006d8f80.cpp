#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(char, __stdcall, GfxPTank_RegisterParticle, 0x006D8710, (int* a1));

static std::uint32_t __fastcall S_006d8f80_UpdateParticlePositions(
    S_006d8f80* self, void* /*edx*/, int* source)
{
    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t planarBytes[0x8];
    alignas(8) std::uint8_t matrixResultBytes[0x10];

    void* sourceObject =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);
    void* sourcePosition =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, sourceObject, 0)[0x10 / 4])(sourceObject, nullptr);

    sourceObject =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, source, 0)[0x14 / 4])(source, nullptr);
    void* matrixResult =
        ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, sourceObject, 0)[0x8C / 4])(
                sourceObject, nullptr, matrixResultBytes);
    auto* matrixPosition =
        static_cast<std::uint8_t*>(CSF_AT(void*, matrixResult, 0)) + 0x20;

    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    const float z = CSF_AT(float, sourcePosition, 0x8);
    const std::uint32_t xBits = CSF_AT(std::uint32_t, sourcePosition, 0);
    const float y = CSF_AT(float, sourcePosition, 0x4);
    CSF_AT(std::uint32_t, positionBytes, 0) = xBits;
    std::uint32_t count = self->field_00e0;
    CSF_AT(float, positionBytes, 0x4) = y;
    CSF_AT(float, positionBytes, 0x8) = z;

    for (std::uint32_t index = 0; index < count; ++index)
    {
        void* emitter = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_00d8), 4 * index);
        auto* particles = static_cast<std::uint8_t*>(emitter) + 0x20;
        const std::uint32_t particleIndex = self->field_00f4;
        const std::uint32_t available =
            ((std::uint32_t (__fastcall*)(void*, void*))
                CSF_AT(void**, particles, 0)[0x14 / 4])(particles, nullptr);

        emitter = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_00d8), 4 * index);
        particles = static_cast<std::uint8_t*>(emitter) + 0x20;

        void* particle;
        if (particleIndex < available)
        {
            particle =
                ((void* (__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, particles, 0)[0x18 / 4])(
                        particles, nullptr, particleIndex);
        }
        else
        {
            particle =
                ((void* (__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, particles, 0)[0xC / 4])(
                        particles, nullptr, sourcePosition);
        }

        if (!self->field_00f4)
        {
            emitter = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_00d8), 4 * index);
            particles = static_cast<std::uint8_t*>(emitter) + 0x20;
            void* initialParticle =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, particles, 0)[0x8 / 4])(particles, nullptr);

            const std::uint32_t planarZ =
                CSF_AT(std::uint32_t, matrixPosition, 0x8);
            const std::uint32_t planarX =
                CSF_AT(std::uint32_t, matrixPosition, 0);
            CSF_AT(std::uint32_t, planarBytes, 0x4) = planarZ;
            CSF_AT(std::uint32_t, planarBytes, 0) = planarX;

            auto* initializer =
                static_cast<std::uint8_t*>(initialParticle) + 0x20;
            ((void (__fastcall*)(void*, void*, void*, void*, int))
                CSF_AT(void**, initializer, 0)[0x14 / 4])(
                    initializer, nullptr, positionBytes, planarBytes, 1);
            GfxPTank_RegisterParticle(static_cast<int*>(initialParticle));
        }

        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, particle, 0)[0x38 / 4])(
                particle, nullptr, positionBytes);
        count = self->field_00e0;
    }

    return count;
}

HOOK(0x006D8F80, S_006d8f80_UpdateParticlePositions, "auto");
