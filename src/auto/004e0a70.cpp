#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, SceneGeometry_GetVertexById, 0x004DE860, (void* self, void* edx, int a2));

static bool __fastcall S_004e0a70_ContainsProjectedPoint(
    S_004e0a70* self, void* /*edx*/, float* point)
{
    bool inside = false;
    void* geometry = reinterpret_cast<void*>(
        CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr));

    const uint32_t vertexCount = CSF_AT(uint32_t, self, 0x8);
    uint32_t nextIndex = 0;
    uint32_t byteOffset = 0;

    while (nextIndex < vertexCount)
    {
        float* current = reinterpret_cast<float*>(
            SceneGeometry_GetVertexById(
                geometry, nullptr,
                CSF_AT(int, CSF_AT(void*, self, 0xC), byteOffset)));

        ++nextIndex;
        float* next = reinterpret_cast<float*>(
            SceneGeometry_GetVertexById(
                geometry, nullptr,
                CSF_AT(int, CSF_AT(void*, self, 0xC),
                       16 * (nextIndex % vertexCount))));

        if (next[2] != current[2] &&
            ((current[2] >= point[1] && next[2] <= point[1]) ||
             (current[2] <= point[1] && next[2] >= point[1])) &&
            (next[0] - current[0]) * (point[1] - current[2]) /
                    (next[2] - current[2]) + current[0] > point[0])
        {
            inside = !inside;
        }

        byteOffset += 16;
    }

    return inside;
}

HOOK(0x004E0A70, S_004e0a70_ContainsProjectedPoint, "auto");
