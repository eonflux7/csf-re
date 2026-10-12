#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));

static int __fastcall GfxClumpLoader_ApplyRenderParameters(
    void* self, void* /*edx*/, int** renderObjectPtr,
    std::uint32_t* records, int /*unused*/)
{
    const std::uint32_t initialCount = CSF_AT(std::uint32_t, records, 0x08);
    int* renderObject = *renderObjectPtr;
    std::uint32_t processed = 0;
    std::uint32_t byteOffset = 0;

    if (initialCount != 0)
    {
        alignas(8) std::uint8_t displacementBytes[0x18];
        float* displacement = reinterpret_cast<float*>(displacementBytes);

        do
        {
            std::uint8_t* record =
                CSF_AT(std::uint8_t*, records, 0x00) + byteOffset;
            const float displacementScale = CSF_AT(float, record, 0x04);

            void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
            void* renderHandle =
                reinterpret_cast<void*>(RenderHandle_FromObject(globalObject, nullptr));
            float* direction =
                (reinterpret_cast<float* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, renderHandle, 0x00)[0x14 / 4]))(
                    renderHandle, nullptr);

            const float directionX = -CSF_AT(float, direction, 0x00);
            const float directionY = -CSF_AT(float, direction, 0x04);
            const float directionZ = -CSF_AT(float, direction, 0x08);
            const float scale = displacementScale * CSF_AT(float, self, 0x04);

            displacement[0] = directionX * scale;
            displacement[1] = scale * directionY;
            displacement[2] = scale * directionZ;

            (reinterpret_cast<void (__fastcall*)(
                void*, void*, float*, void*, std::uint32_t)>(
                    CSF_AT(void**, renderObject, 0x00)[0x40 / 4]))(
                        renderObject, nullptr, displacement, record + 0x0C,
                        CSF_AT(std::uint32_t, record, 0x00));

            const float firstValue =
                CSF_AT(float, self, 0x14) * CSF_AT(float, record, 0x08);
            (reinterpret_cast<void (__fastcall*)(
                void*, void*, float, std::uint32_t)>(
                    CSF_AT(void**, renderObject, 0x00)[0x54 / 4]))(
                        renderObject, nullptr, firstValue,
                        CSF_AT(std::uint32_t, record, 0x00));

            const float secondValue =
                CSF_AT(float, self, 0x18) * CSF_AT(float, record, 0x08);
            (reinterpret_cast<void (__fastcall*)(
                void*, void*, float, std::uint32_t)>(
                    CSF_AT(void**, renderObject, 0x00)[0x58 / 4]))(
                        renderObject, nullptr, secondValue,
                        CSF_AT(std::uint32_t, record, 0x00));

            ++processed;
            byteOffset += 0x18;
        }
        while (processed < CSF_AT(std::uint32_t, records, 0x08));
    }

    return static_cast<int>(processed);
}

HOOK(0x006FEA00, GfxClumpLoader_ApplyRenderParameters, "auto");
