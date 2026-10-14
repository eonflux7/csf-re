#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __fastcall, S_00692b80_ReadIntegerProperty, 0x00693910, (S_00692b80* self, void* , char* keyword));
CSF_FUNC(double, __fastcall, S_00692b80_ReadFloatProperty, 0x00693940, (S_00692b80* self, void* , char* keyword));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_RadarTwoPi, 0x00830E28);
CSF_GLOBAL(float, g_RadarAngleLoopLimit, 0x00830E2C);
CSF_GLOBAL(const char*, g_pszKey_NUM_VERTEX, 0x00899698);
CSF_GLOBAL(char*, g_pszKey_ColorComponent0, 0x0089969C);
CSF_GLOBAL(char*, g_pszKey_ColorComponent1, 0x008996A0);
CSF_GLOBAL(char*, g_pszKey_ColorComponent2, 0x008996A4);
CSF_GLOBAL(char*, g_pszKey_ColorComponent3, 0x008996A8);
CSF_GLOBAL(const char*, g_pszKey_SOLID, 0x008996AC);

static void Minimap_AddMeshVertex(void* self, float* position, float* color)
{
    void* mesh = CSF_AT(void*, self, 0x1C);
    ((void(__fastcall*)(void*, void*, float*, float*))
        CSF_AT(void**, mesh, 0)[12])(mesh, nullptr, position, color);
}

static void Minimap_AddMeshIndex(void* self, int index)
{
    void* mesh = CSF_AT(void*, self, 0x1C);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, mesh, 0)[14])(mesh, nullptr, index);
}

static void __fastcall Minimap_SetupFromComponent(
    void* self, void* /*edx*/, S_00692b80* properties)
{
    if (properties == nullptr)
        return;

    int vertexCount = S_00692b80_ReadIntegerProperty(
        properties, nullptr, const_cast<char*>(g_pszKey_NUM_VERTEX));
    CSF_AT(int, self, 0x20) = vertexCount;
    if (vertexCount < 5)
        vertexCount = 5;
    CSF_AT(int, self, 0x20) = vertexCount;

    {
        void* mesh = CSF_AT(void*, self, 0x1C);
        // 0043BB77 saves EBX until 0043BF0B; it is not a slot argument.
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, mesh, 0)[10])(mesh, nullptr);
    }
    {
        const int count = CSF_AT(int, self, 0x20);
        void* mesh = CSF_AT(void*, self, 0x1C);
        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, mesh, 0)[7])(mesh, nullptr, 2 * count + 1, 9 * count);
    }
    {
        void* mesh = CSF_AT(void*, self, 0x1C);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, mesh, 0)[8])(mesh, nullptr, 1);
    }

    const int left = CSF_AT(int, self, 0x24);
    const int right = CSF_AT(int, self, 0x2C);
    const int top = CSF_AT(int, self, 0x28);
    const int bottom = CSF_AT(int, self, 0x30);
    const int width = right - left;
    const int height = bottom - top;

    const float outerRadiusX = static_cast<float>(static_cast<double>(width + 10) * g_HalfDurationScale);
    const float outerRadiusY = static_cast<float>(static_cast<double>(height + 10) * g_HalfDurationScale);
    const float centerX =
        static_cast<float>(static_cast<double>(width) * g_HalfDurationScale + left);
    const float centerY =
        static_cast<float>(static_cast<double>(height) * g_HalfDurationScale + top);

    CSF_AT(float, self, 0x3C) = static_cast<float>(
        S_00692b80_ReadFloatProperty(
            properties, nullptr, const_cast<char*>(g_pszKey_SOLID)));

    float upperBound = 1.0f;
    float lowerBound = 0.0f;
    ClampFloatToBounds(&CSF_AT(float, self, 0x3C), &lowerBound, &upperBound);

    const float innerRadiusX = outerRadiusX * CSF_AT(float, self, 0x3C);
    const float innerRadiusY = outerRadiusY * CSF_AT(float, self, 0x3C);

    float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    color[3] = static_cast<float>(S_00692b80_ReadIntegerProperty(
        properties, nullptr, g_pszKey_ColorComponent3));
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[3], &lowerBound, &upperBound);

    color[0] = static_cast<float>(S_00692b80_ReadIntegerProperty(
        properties, nullptr, g_pszKey_ColorComponent0));
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[0], &lowerBound, &upperBound);

    color[1] = static_cast<float>(S_00692b80_ReadIntegerProperty(
        properties, nullptr, g_pszKey_ColorComponent1));
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[1], &lowerBound, &upperBound);

    color[2] = static_cast<float>(S_00692b80_ReadIntegerProperty(
        properties, nullptr, g_pszKey_ColorComponent2));
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[2], &lowerBound, &upperBound);

    alignas(8) std::uint8_t screenScaleBytes[0x14];
    float* screenScale = reinterpret_cast<float*>(screenScaleBytes);
    screenScale[0] = 0.0f;
    screenScale[1] = 0.0f;
    Hud_WriteNormalizedScreenSize(screenScale);

    float position[2];
    position[0] = screenScale[0] * centerX;
    position[1] = screenScale[1] * centerY;
    Minimap_AddMeshVertex(self, position, color);

    const float angleStep = static_cast<float>(
        static_cast<double>(g_RadarTwoPi) / CSF_AT(int, self, 0x20));
    const float ringAlpha = color[3];
    float angle = 0.0f;

    do
    {
        const double cosAngle = std::cos(static_cast<double>(angle));
        color[3] = ringAlpha;
        position[0] = (cosAngle * innerRadiusX + centerX) * screenScale[0];
        const double sinAngle = std::sin(static_cast<double>(angle));
        position[1] = (sinAngle * innerRadiusY + centerY) * screenScale[1];
        Minimap_AddMeshVertex(self, position, color);

        color[3] = 0.0f;
        position[0] = (static_cast<double>(static_cast<float>(cosAngle)) *
                       outerRadiusX + centerX) * screenScale[0];
        position[1] = (static_cast<double>(static_cast<float>(sinAngle)) *
                       outerRadiusY + centerY) * screenScale[1];
        Minimap_AddMeshVertex(self, position, color);

        const double nextAngle = static_cast<double>(angle) + angleStep;
        angle = static_cast<float>(nextAngle);
        if (!(nextAngle < g_RadarAngleLoopLimit))
            break;
    } while (true);

    int lastBase = 0;
    int baseIndex = 0;
    unsigned int segment = 0;

    if (CSF_AT(int, self, 0x20) - 1 != 0)
    {
        int vertexIndex = 3;
        do
        {
            Minimap_AddMeshIndex(self, vertexIndex - 2);
            Minimap_AddMeshIndex(self, 0);
            Minimap_AddMeshIndex(self, vertexIndex);
            Minimap_AddMeshIndex(self, vertexIndex - 1);
            Minimap_AddMeshIndex(self, vertexIndex - 2);
            Minimap_AddMeshIndex(self, vertexIndex);
            Minimap_AddMeshIndex(self, vertexIndex);
            Minimap_AddMeshIndex(self, vertexIndex + 1);
            Minimap_AddMeshIndex(self, vertexIndex - 1);

            lastBase = baseIndex + 2;
            vertexIndex += 2;
            baseIndex += 2;
            ++segment;
        } while (segment < static_cast<unsigned int>(CSF_AT(int, self, 0x20) - 1));
    }

    const int lastInner = lastBase + 1;
    Minimap_AddMeshIndex(self, lastInner);
    Minimap_AddMeshIndex(self, 0);
    Minimap_AddMeshIndex(self, 1);

    const int lastOuter = lastBase + 2;
    Minimap_AddMeshIndex(self, lastOuter);
    Minimap_AddMeshIndex(self, lastInner);
    Minimap_AddMeshIndex(self, 1);
    Minimap_AddMeshIndex(self, 1);
    Minimap_AddMeshIndex(self, 2);
    Minimap_AddMeshIndex(self, lastOuter);
}

HOOK(0x0043BB40, Minimap_SetupFromComponent, "auto");
