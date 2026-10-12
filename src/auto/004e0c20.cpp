#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, Geometry_ComputeTriangleResult, 0x004020F0,
         (void* self, void* edx, float* arg0, float* a3, float* a4));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630,
         (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, SceneGeometry_GetVertex, 0x004DE860,
         (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, Geometry_ContainsPointInTriangle, 0x006AFCA0,
         (void* self, void* edx, float* a2));

static char __fastcall S_004e0c20_FindContainingTriangle(
    S_004e0c20* self, void* /*edx*/, uint32_t* point, float* output)
{
    alignas(8) uint8_t triangleBytes[0x18] = {};
    alignas(8) uint8_t pointBytes[0x30];

    char found = 0;
    uint32_t triangleIndex = 0;
    uint32_t vertexOffset = 0;

    if (self->field_0008 - 2 == 0)
        return found;

    while (!found)
    {
        int vertexId = CSF_AT(int, CSF_AT(uint8_t*, self, 0x0C), 0);
        uint32_t geometry = CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
        float* first = reinterpret_cast<float*>(SceneGeometry_GetVertex(
            reinterpret_cast<void*>(geometry), nullptr, vertexId));

        vertexId = CSF_AT(
            int, CSF_AT(uint8_t*, self, 0x0C), vertexOffset + 0x10);
        geometry = CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
        float* second = reinterpret_cast<float*>(SceneGeometry_GetVertex(
            reinterpret_cast<void*>(geometry), nullptr, vertexId));

        vertexId = CSF_AT(
            int, CSF_AT(uint8_t*, self, 0x0C), vertexOffset + 0x20);
        geometry = CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
        float* third = reinterpret_cast<float*>(SceneGeometry_GetVertex(
            reinterpret_cast<void*>(geometry), nullptr, vertexId));

        const uint32_t firstX = CSF_AT(uint32_t, first, 0);
        const uint32_t firstZ = CSF_AT(uint32_t, first, 8);
        CSF_AT(uint32_t, pointBytes, 0x18) = firstX;
        CSF_AT(uint32_t, pointBytes, 0x1C) = firstZ;
        CSF_AT(uint32_t, triangleBytes, 0x00) = firstX;
        CSF_AT(uint32_t, triangleBytes, 0x04) = firstZ;

        const uint32_t secondZ = CSF_AT(uint32_t, second, 8);
        const uint32_t secondX = CSF_AT(uint32_t, second, 0);
        CSF_AT(uint32_t, pointBytes, 0x10) = secondX;
        CSF_AT(uint32_t, pointBytes, 0x14) = secondZ;
        CSF_AT(uint32_t, triangleBytes, 0x08) = secondX;
        CSF_AT(uint32_t, triangleBytes, 0x0C) = secondZ;

        const uint32_t thirdX = CSF_AT(uint32_t, third, 0);
        const uint32_t thirdZ = CSF_AT(uint32_t, third, 8);
        CSF_AT(uint32_t, pointBytes, 0x08) = thirdX;
        CSF_AT(uint32_t, pointBytes, 0x0C) = thirdZ;
        CSF_AT(uint32_t, triangleBytes, 0x10) = thirdX;
        CSF_AT(uint32_t, triangleBytes, 0x14) = thirdZ;

        CSF_AT(uint32_t, pointBytes, 0x00) = point[0];
        CSF_AT(uint32_t, pointBytes, 0x04) = point[2];

        if (Geometry_ContainsPointInTriangle(
                triangleBytes, nullptr, reinterpret_cast<float*>(pointBytes)))
        {
            Geometry_ComputeTriangleResult(
                output, nullptr, first, second, third);
            found = 1;
        }

        const uint32_t triangleCount = self->field_0008 - 2;
        ++triangleIndex;
        vertexOffset += 0x10;
        if (triangleIndex >= triangleCount)
            break;
    }

    return found;
}

HOOK(0x004E0C20, S_004e0c20_FindContainingTriangle, "auto");
