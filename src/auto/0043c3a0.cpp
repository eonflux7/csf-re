#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// ---------------------------------------------------------------------------
// Callees
// ---------------------------------------------------------------------------
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));

// ---------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);        // 0.5f
CSF_GLOBAL(float, g_RadarAngleLoopLimit, 0x00830E2C);      // 2*pi - 0.001
CSF_GLOBAL(float, g_RadarTwoPi, 0x00830E44);               // 2*pi
CSF_GLOBAL(char*, g_pszKey_NUM_VERTEX, 0x00899698);
CSF_GLOBAL(char*, g_pszKey_ColorComponent0, 0x0089969C);
CSF_GLOBAL(char*, g_pszKey_ColorComponent1, 0x008996A0);
CSF_GLOBAL(char*, g_pszKey_ColorComponent2, 0x008996A4);
CSF_GLOBAL(char*, g_pszKey_ColorComponent3, 0x008996A8);
CSF_GLOBAL(char*, g_pszKey_SOLID, 0x008996AC);

// ---------------------------------------------------------------------------
// Small wrappers around the mesh builder stored at self+0x1C. The pointer is
// re-read from the object on every call, as the original does.
// ---------------------------------------------------------------------------
static inline void MinimapRadar_AddMeshVertex(void* self, float* position, float* color)
{
    void* mesh = CSF_AT(void*, self, 0x1C);
    ((void(__fastcall*)(void*, void*, float*, float*))CSF_AT(void**, mesh, 0)[12])(mesh, nullptr, position, color);
}

static inline void MinimapRadar_AddMeshIndex(void* self, int index)
{
    void* mesh = CSF_AT(void*, self, 0x1C);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, mesh, 0)[14])(mesh, nullptr, index);
}

// ---------------------------------------------------------------------------
// 0x0043C3A0 - builds the elliptical radar ring mesh (center vertex + pairs of
// inner/outer vertices, triangle list indices) from the property block.
// ---------------------------------------------------------------------------
static void __fastcall MinimapRadar_BuildRingMesh(void* self, void* /*edx*/, std::uint8_t* properties)
{
    if (properties == nullptr)
        return;

    int numVertex = ReadIntegerProperty(properties, nullptr, g_pszKey_NUM_VERTEX);
    CSF_AT(int, self, 0x20) = numVertex;
    if (numVertex < 5)
        numVertex = 5;
    CSF_AT(int, self, 0x20) = numVertex;

    {
        void* mesh = CSF_AT(void*, self, 0x1C);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, mesh, 0)[10])(mesh, nullptr);
    }
    {
        int count = CSF_AT(int, self, 0x20);
        void* mesh = CSF_AT(void*, self, 0x1C);
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, mesh, 0)[7])(mesh, nullptr, 2 * count + 1, 9 * count);
    }
    {
        void* mesh = CSF_AT(void*, self, 0x1C);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, mesh, 0)[8])(mesh, nullptr, 1);
    }

    const int left = CSF_AT(int, self, 0x24);
    const int right = CSF_AT(int, self, 0x2C);
    const int top = CSF_AT(int, self, 0x28);
    const int bottom = CSF_AT(int, self, 0x30);

    const int width = right - left;
    const int height = bottom - top;

    const float outerRadiusX = (float)(width + 10) * g_HalfDurationScale;
    const float outerRadiusY = (float)(height + 10) * g_HalfDurationScale;
    const float centerX = (float)width * g_HalfDurationScale + (float)left;
    const float centerY = (float)height * g_HalfDurationScale + (float)top;

    float lowerBound;
    float upperBound;

    CSF_AT(float, self, 0x3C) = (float)ReadFloatProperty(properties, nullptr, g_pszKey_SOLID);
    upperBound = 1.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&CSF_AT(float, self, 0x3C), &lowerBound, &upperBound);

    const float innerRadiusX = outerRadiusX * CSF_AT(float, self, 0x3C);
    const float innerRadiusY = outerRadiusY * CSF_AT(float, self, 0x3C);

    float color[4];
    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 1.0f;

    color[3] = (float)ReadIntegerProperty(properties, nullptr, g_pszKey_ColorComponent3);
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[3], &lowerBound, &upperBound);

    color[0] = (float)ReadIntegerProperty(properties, nullptr, g_pszKey_ColorComponent0);
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[0], &lowerBound, &upperBound);

    color[1] = (float)ReadIntegerProperty(properties, nullptr, g_pszKey_ColorComponent1);
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[1], &lowerBound, &upperBound);

    color[2] = (float)ReadIntegerProperty(properties, nullptr, g_pszKey_ColorComponent2);
    upperBound = 255.0f;
    lowerBound = 0.0f;
    ClampFloatToBounds(&color[2], &lowerBound, &upperBound);

    float screenScale[2];
    screenScale[0] = 0.0f;
    screenScale[1] = 0.0f;
    Hud_WriteNormalizedScreenSize(screenScale);

    float position[2];

    // Center vertex.
    position[0] = screenScale[0] * centerX;
    position[1] = screenScale[1] * centerY;
    MinimapRadar_AddMeshVertex(self, position, color);

    const float angleStep = g_RadarTwoPi / (float)CSF_AT(int, self, 0x20);
    const float ringAlpha = color[3];
    float angle = 0.0f;

    do
    {
        const float cosAngle = std::cos(angle);
        color[3] = ringAlpha;
        position[0] = (cosAngle * innerRadiusX + centerX) * screenScale[0];
        const float sinAngle = std::sin(angle);
        position[1] = (sinAngle * innerRadiusY + centerY) * screenScale[1];
        MinimapRadar_AddMeshVertex(self, position, color);

        color[3] = 0.0f;
        position[0] = (cosAngle * outerRadiusX + centerX) * screenScale[0];
        position[1] = (sinAngle * outerRadiusY + centerY) * screenScale[1];
        MinimapRadar_AddMeshVertex(self, position, color);

        angle = angle + angleStep;
    } while (angle < g_RadarAngleLoopLimit);

    int lastBase = 0;
    int baseIndex = 0;
    unsigned int segment = 0;

    if (CSF_AT(int, self, 0x20) - 1 != 0)
    {
        int vertexIndex = 3;
        do
        {
            // Fan triangle to the center.
            MinimapRadar_AddMeshIndex(self, vertexIndex - 2);
            MinimapRadar_AddMeshIndex(self, 0);
            MinimapRadar_AddMeshIndex(self, vertexIndex);
            // Fading outer quad.
            MinimapRadar_AddMeshIndex(self, vertexIndex - 1);
            MinimapRadar_AddMeshIndex(self, vertexIndex - 2);
            MinimapRadar_AddMeshIndex(self, vertexIndex);
            MinimapRadar_AddMeshIndex(self, vertexIndex);
            MinimapRadar_AddMeshIndex(self, vertexIndex + 1);
            MinimapRadar_AddMeshIndex(self, vertexIndex - 1);

            lastBase = baseIndex + 2;
            vertexIndex += 2;
            baseIndex += 2;
            ++segment;
        } while (segment < (unsigned int)(CSF_AT(int, self, 0x20) - 1));
    }

    // Closing segment (wraps around to the first ring vertices).
    const int lastInner = lastBase + 1;
    MinimapRadar_AddMeshIndex(self, lastInner);
    MinimapRadar_AddMeshIndex(self, 0);
    MinimapRadar_AddMeshIndex(self, 1);

    const int lastOuter = lastBase + 2;
    MinimapRadar_AddMeshIndex(self, lastOuter);
    MinimapRadar_AddMeshIndex(self, lastInner);
    MinimapRadar_AddMeshIndex(self, 1);
    MinimapRadar_AddMeshIndex(self, 1);
    MinimapRadar_AddMeshIndex(self, 2);
    MinimapRadar_AddMeshIndex(self, lastOuter);
}

HOOK(0x0043C3A0, MinimapRadar_BuildRingMesh, "auto");
