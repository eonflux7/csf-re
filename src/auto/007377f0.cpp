#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));

static int __fastcall Geometry_TransformQuadVertices(void* self, void* /*edx*/)
{
    alignas(8) uint8_t outputBytes[0xC];
    alignas(8) uint8_t fourthCornerBytes[0xC];
    alignas(8) uint8_t thirdCornerBytes[0xC];
    alignas(8) uint8_t secondCornerBytes[0xC];
    alignas(8) uint8_t firstCornerBytes[0x18];

    float* output = reinterpret_cast<float*>(outputBytes);
    float* first = reinterpret_cast<float*>(firstCornerBytes);
    float* second = reinterpret_cast<float*>(secondCornerBytes);
    float* third = reinterpret_cast<float*>(thirdCornerBytes);
    float* fourth = reinterpret_cast<float*>(fourthCornerBytes);

    const float left = CSF_AT(float, self, 0x34);
    const float height = CSF_AT(float, self, 0x50);
    const float top = CSF_AT(float, self, 0x38);
    const float width = CSF_AT(float, self, 0x4C);
    const float right = left + width;
    const float bottom = top + height;

    if (CSF_AT(uint8_t, self, 0x8C))
    {
        first[0] = right;
        first[1] = top;
        second[0] = left;
        second[1] = top;
        third[0] = right;
        third[1] = bottom;
        fourth[0] = left;
        fourth[1] = bottom;
    }
    else
    {
        first[0] = left;
        first[1] = top;
        second[0] = left;
        second[1] = bottom;
        third[0] = right;
        third[1] = top;
        fourth[0] = right;
        fourth[1] = bottom;
    }
    first[2] = 0;
    second[2] = 0;
    third[2] = 0;
    fourth[2] = 0;

    RwV3dTransformPoints(
        reinterpret_cast<int>(output),
        reinterpret_cast<int>(first),
        1, CSF_AT(int, self, 0x84));
    {
        const float x = output[0];
        const float y = output[1];
        void* offsetObject = CSF_AT(void*, self, 0x0C);
        const double xOffset =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
        CSF_AT(float, CSF_AT(void*, self, 0x14), 0) = x - xOffset;

        offsetObject = CSF_AT(void*, self, 0x0C);
        const double yOffset =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
        CSF_AT(float, CSF_AT(void*, self, 0x14), 4) = y - yOffset;
    }

    RwV3dTransformPoints(
        reinterpret_cast<int>(output),
        reinterpret_cast<int>(second),
        1, CSF_AT(int, self, 0x84));
    {
        const float x = output[0];
        const float y = output[1];
        void* offsetObject = CSF_AT(void*, self, 0x0C);
        const double xOffset =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
        CSF_AT(float, CSF_AT(void*, self, 0x14), 28) = x - xOffset;

        offsetObject = CSF_AT(void*, self, 0x0C);
        const double yOffset =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
        CSF_AT(float, CSF_AT(void*, self, 0x14), 32) = y - yOffset;
    }

    RwV3dTransformPoints(
        reinterpret_cast<int>(output),
        reinterpret_cast<int>(third),
        1, CSF_AT(int, self, 0x84));
    {
        const float x = output[0];
        const float y = output[1];
        void* offsetObject = CSF_AT(void*, self, 0x0C);
        const double xOffset =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
        CSF_AT(float, CSF_AT(void*, self, 0x14), 56) = x - xOffset;

        offsetObject = CSF_AT(void*, self, 0x0C);
        const double yOffset =
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
        CSF_AT(float, CSF_AT(void*, self, 0x14), 60) = y - yOffset;
    }

    RwV3dTransformPoints(
        reinterpret_cast<int>(output),
        reinterpret_cast<int>(fourth),
        1, CSF_AT(int, self, 0x84));
    const float x = output[0];
    const float y = output[1];
    void* offsetObject = CSF_AT(void*, self, 0x0C);
    const double xOffset =
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
    CSF_AT(float, CSF_AT(void*, self, 0x14), 84) = x - xOffset;

    offsetObject = CSF_AT(void*, self, 0x0C);
    const double yOffset =
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, offsetObject, 0)[32 / 4])(offsetObject, nullptr);
    void* vertices = CSF_AT(void*, self, 0x14);
    CSF_AT(float, vertices, 88) = y - yOffset;
    return reinterpret_cast<int>(vertices);
}

HOOK(0x007377F0, Geometry_TransformQuadVertices, "auto");
