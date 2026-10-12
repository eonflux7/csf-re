#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Radar ring vertex (32 bytes): [0] u, [1] v, [2] screen x, [3] screen y, [4..7] colour (r, g, b, a as floats).

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __cdecl, Minimap_ComputeRadarExtent, 0x0043CD00, (float* a1, int* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(unsigned int, __fastcall, MinimapVertexVector_PushBack, 0x0043DAF0, (void* self, void* edx, void* a3));
CSF_FUNC(int, __cdecl, MinimapVertex_UninitializedFillN, 0x0043D330, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, MinimapVertexVector_InsertN, 0x0043D890, (void* self, void* edx, int a1, unsigned int a3, void* arg8));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_RadarTwoPi, 0x00830E54);
CSF_GLOBAL(float, g_RadarTwoPiMinusEpsilon, 0x00830E2C);

// Rebuilds the triangle-fan vertices of the circular multiplayer radar: a centre vertex on the
// player's map position followed by pairs (opaque inner ring / transparent outer ring) around it.
static void __fastcall C_00830e5c_RebuildRadarRingVertices(C_00830e5c* self, void* /*edx*/)
{
    alignas(8) uint8_t vertexBytes[0x44];
    float* vertex = reinterpret_cast<float*>(vertexBytes);
    alignas(8) uint8_t extentBytes[0x3C];
    float* extent = reinterpret_cast<float*>(extentBytes);

    // Map bounds of the current level.
    uint8_t* levelForBoundZ = reinterpret_cast<uint8_t*>(GetValue8B0554());
    uint8_t* levelForBoundX = reinterpret_cast<uint8_t*>(GetValue8B0554());
    extent[0] = CSF_AT(float, levelForBoundX, 0xDB4);
    extent[1] = CSF_AT(float, levelForBoundZ, 0xDBC);
    uint8_t* levelForOppositeZ = reinterpret_cast<uint8_t*>(GetValue8B0554());
    uint8_t* levelForOppositeX = reinterpret_cast<uint8_t*>(GetValue8B0554());
    float oppositeBoundX = CSF_AT(float, levelForOppositeX, 0xDC0);
    float oppositeBoundZ = CSF_AT(float, levelForOppositeZ, 0xDC8);

    // Player position normalised into map texture space.
    void* sceneContext = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    float* position = GetObjectPositionVector(sceneContext, nullptr);
    float positionX = position[0];
    float positionZ = position[2];
    float centerU = (extent[0] - positionX) / (extent[0] - oppositeBoundX);
    float centerV = (extent[1] - positionZ) / (extent[1] - oppositeBoundZ);

    Minimap_ComputeRadarExtent(extent, reinterpret_cast<int*>(&self->field_0034));

    float ringScale = *reinterpret_cast<float*>(&self->field_003c);
    float scaledExtentU = extent[0] * ringScale;
    float scaledExtentV = extent[1] * ringScale;
    float invTextureWidth = g_TransitionOne / (float)(int)self->field_0034;
    float invTextureHeight = g_TransitionOne / (float)(int)self->field_0038;

    int left = (int)self->field_0024;
    int top = (int)self->field_0028;
    int right = (int)self->field_002c;
    int bottom = (int)self->field_0030;

    float outerRadiusU = extent[0] * invTextureWidth;
    float outerRadiusV = invTextureHeight * extent[1];
    float innerRadiusU = invTextureWidth * scaledExtentU;
    float innerRadiusV = invTextureHeight * scaledExtentV;

    int centerXInt = left + (right - left) / 2;
    int centerYInt = top + (bottom - top) / 2;
    int halfWidth = (right - left + 10) / 2;
    int halfHeight = (bottom - top + 10) / 2;

    float outerRadiusX = (float)halfWidth;
    int innerRadiusXInt = (int)(outerRadiusX * ringScale);
    float outerRadiusY = (float)halfHeight;
    int innerRadiusYInt = (int)(outerRadiusY * ringScale);

    uint8_t* vertexList = self->pad_004c;

    // Clear the vertex vector.
    if (self->field_0050 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0050));
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;

    float centerX = (float)centerXInt;
    float centerY = (float)centerYInt;

    // Centre vertex, fully opaque white.
    vertex[0] = centerU;
    vertex[1] = centerV;
    vertex[2] = centerX;
    vertex[3] = centerY;
    vertex[4] = 255.0f;
    vertex[5] = 255.0f;
    vertex[6] = 255.0f;
    vertex[7] = 255.0f;
    MinimapVertexVector_PushBack(vertexList, nullptr, vertex);

    // Inlined vector::insert(end(), 1, vertex).
    auto appendVertex = [self, vertexList, vertex]() {
        uint32_t begin = self->field_0050;
        uint32_t count = begin ? (self->field_0054 - begin) >> 5 : 0;
        if (begin && count < (self->field_0058 - begin) >> 5)
        {
            uint32_t end = self->field_0054;
            // Last argument is an empty iterator-category tag (garbage in the original).
            MinimapVertex_UninitializedFillN((int)end, 1, reinterpret_cast<int>(vertex),
                                             reinterpret_cast<int>(vertexList), 0);
            self->field_0054 = end + 32;
        }
        else
        {
            MinimapVertexVector_InsertN(vertexList, nullptr, (int)self->field_0054, 1, vertex);
        }
    };

    float angle = 0.0f;
    float angleStep = g_RadarTwoPi / (float)(int)self->field_0020;
    float innerRadiusX = (float)innerRadiusXInt;
    float innerRadiusY = (float)innerRadiusYInt;

    do
    {
        // Inner ring vertex: opaque.
        vertex[7] = 255.0f;
        float cosAngle = std::cos(angle);
        vertex[2] = innerRadiusX * cosAngle + centerX;
        float sinAngle = std::sin(angle);
        vertex[3] = innerRadiusY * sinAngle + centerY;
        vertex[0] = cosAngle * innerRadiusU + centerU;
        vertex[1] = sinAngle * innerRadiusV + centerV;
        appendVertex();

        // Outer ring vertex: transparent.
        vertex[7] = 0.0f;
        vertex[2] = cosAngle * outerRadiusX + centerX;
        vertex[3] = sinAngle * outerRadiusY + centerY;
        vertex[0] = cosAngle * outerRadiusU + centerU;
        vertex[1] = sinAngle * outerRadiusV + centerV;
        appendVertex();

        angle = angle + angleStep;
    } while (angle < g_RadarTwoPiMinusEpsilon);
}

HOOK(0x0043DBC0, C_00830e5c_RebuildRadarRingVertices, "auto");
