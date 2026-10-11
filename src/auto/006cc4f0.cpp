#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GfxScene_GrowVisibleClumpList, 0x006CAFE0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxScene_ContainsBounds, 0x006CB260, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(char, __fastcall, GfxScene_UpdateClumpView, 0x006F63C0, (void* self, void* edx, int a2, int a3, float a4));

static float** __fastcall S_006cc4f0_BuildVisibleClumpList(
    S_006cc4f0* self, void* /*edx*/)
{
    alignas(8) std::uint8_t boundsBytes[0x18];
    alignas(8) std::uint8_t positionBytes[0xC];
    alignas(8) std::uint8_t directionBytes[0xC];
    alignas(8) std::uint8_t upBytes[0x1C];
    alignas(8) std::uint8_t matrixBytes[0x10];

    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* view = CSF_AT(void*, globals, 0xF0);
    void* camera =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, view, 0)[0x10 / 4])(view, nullptr);

    void* position =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, camera, 0)[0x10 / 4])(camera, nullptr);

    CSF_AT(std::uint32_t, positionBytes, 0) = CSF_AT(std::uint32_t, position, 0);
    CSF_AT(std::uint32_t, positionBytes, 4) = CSF_AT(std::uint32_t, position, 4);
    CSF_AT(std::uint32_t, positionBytes, 8) = CSF_AT(std::uint32_t, position, 8);

    void* transform =
        ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, camera, 0)[0x8C / 4])(camera, nullptr, matrixBytes);
    float* matrix = CSF_AT(float*, transform, 0);

    CSF_AT(float, directionBytes, 0) = -matrix[0];
    CSF_AT(float, directionBytes, 4) = -matrix[1];
    CSF_AT(float, directionBytes, 8) = -matrix[2];

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    transform =
        ((void* (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, camera, 0)[0x8C / 4])(camera, nullptr, matrixBytes);
    void* matrixData = CSF_AT(void*, transform, 0);

    CSF_AT(std::uint32_t, upBytes, 0) = CSF_AT(std::uint32_t, matrixData, 0x20);
    CSF_AT(std::uint32_t, upBytes, 4) = CSF_AT(std::uint32_t, matrixData, 0x24);
    CSF_AT(std::uint32_t, upBytes, 8) = CSF_AT(std::uint32_t, matrixData, 0x28);

    if (CSF_AT(std::uint8_t, matrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    const std::uint32_t initialCount = self->field_007c;
    float** result = nullptr;
    self->field_008c = 0;
    std::uint32_t index = 0;

    if (initialCount)
    {
        do
        {
            auto* entry = reinterpret_cast<std::uint8_t*>(self->field_0074) + 8 * index;
            void* clump = CSF_AT(void*, entry, 0);

            const std::uint32_t minimumX = CSF_AT(std::uint32_t, clump, 0x9C);
            const std::uint32_t minimumY = CSF_AT(std::uint32_t, clump, 0xA0);
            const std::uint32_t minimumZ = CSF_AT(std::uint32_t, clump, 0xA4);
            const std::uint32_t maximumX = CSF_AT(std::uint32_t, clump, 0xA8);
            const std::uint32_t maximumY = CSF_AT(std::uint32_t, clump, 0xAC);
            const std::uint32_t maximumZ = CSF_AT(std::uint32_t, clump, 0xB0);

            CSF_AT(std::uint32_t, boundsBytes, 0) = maximumX;
            CSF_AT(std::uint32_t, boundsBytes, 4) = maximumY;
            CSF_AT(std::uint32_t, boundsBytes, 8) = maximumZ;
            CSF_AT(std::uint32_t, boundsBytes, 0xC) = minimumX;
            CSF_AT(std::uint32_t, boundsBytes, 0x10) = minimumY;
            CSF_AT(std::uint32_t, boundsBytes, 0x14) = minimumZ;

            const bool visible =
                ((bool (__fastcall*)(void*, void*, void*, int, int))
                    CSF_AT(void**, view, 0)[0x70 / 4])(
                        view, nullptr, boundsBytes, 0, 6);

            if (visible)
            {
                CSF_AT(std::uint8_t, entry, 4) = 1;

                // The legacy signature labels this pointer-sized argument as float.
                const std::uint32_t upAddress =
                    reinterpret_cast<std::uintptr_t>(upBytes);
                float upArgument;
                std::memcpy(&upArgument, &upAddress, sizeof(upArgument));

                GfxScene_UpdateClumpView(
                    clump, nullptr,
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(positionBytes)),
                    static_cast<int>(reinterpret_cast<std::uintptr_t>(directionBytes)),
                    upArgument);

                if (self->field_008c >= self->field_0088)
                {
                    GfxScene_GrowVisibleClumpList(
                        &self->field_0084, nullptr, CSF_AT(int, self, 0x90));
                }

                const std::uint32_t count = self->field_008c;
                auto* destination =
                    reinterpret_cast<std::uint8_t*>(self->field_0084) + 8 * count;
                self->field_008c = count + 1;
                CSF_AT(void*, destination, 0) = clump;
                CSF_AT(std::uint32_t, destination, 4) = index;
            }
            else
            {
                CSF_AT(std::uint8_t, entry, 4) = 0;
            }

            const std::uint32_t count = self->field_007c;
            ++index;
            result = reinterpret_cast<float**>(index);
            if (index >= count)
                break;
        } while (true);
    }

    std::int32_t candidateIndex =
        static_cast<std::int32_t>(self->field_008c - 1);
    if (candidateIndex < 0)
        return result;

    do
    {
        auto* savedBase = reinterpret_cast<std::uint8_t*>(self->field_0084);
        result = reinterpret_cast<float**>(savedBase);
        const std::uint32_t count = self->field_008c;
        void* candidate = CSF_AT(void*, savedBase + 8 * candidateIndex, 0);
        std::uint32_t otherIndex = 0;
        auto* cursor = savedBase;

        if (count)
        {
            do
            {
                if (otherIndex != static_cast<std::uint32_t>(candidateIndex) &&
                    GfxScene_ContainsBounds(
                        CSF_AT(void*, cursor, 0), nullptr,
                        reinterpret_cast<float*>(
                            static_cast<std::uint8_t*>(candidate) + 0x9C),
                        reinterpret_cast<float*>(
                            static_cast<std::uint8_t*>(candidate) + 0xA8)))
                {
                    const std::uint32_t currentCount = self->field_008c;
                    auto* currentBase =
                        reinterpret_cast<std::uint8_t*>(self->field_0084);
                    auto* last = currentBase + 8 * currentCount - 8;
                    result = reinterpret_cast<float**>(savedBase);
                    CSF_AT(std::uint32_t, savedBase + 8 * candidateIndex, 0) =
                        CSF_AT(std::uint32_t, last, 0);
                    CSF_AT(std::uint32_t, savedBase + 8 * candidateIndex, 4) =
                        CSF_AT(std::uint32_t, last, 4);
                    --self->field_008c;
                    break;
                }

                const std::uint32_t currentCount = self->field_008c;
                ++otherIndex;
                cursor += 8;
                result = reinterpret_cast<float**>(cursor);
                if (otherIndex >= currentCount)
                    break;
            } while (true);
        }

        --candidateIndex;
    } while (candidateIndex >= 0);

    return result;
}

HOOK(0x006CC4F0, S_006cc4f0_BuildVisibleClumpList, "auto");
