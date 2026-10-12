#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtIntersectionLineTriangle, 0x00620640, (float* a1, float* a2, float* a3, float* a4, float* a5, float* a6));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(bool, __fastcall, GfxScene_MatchesReferencedObject, 0x006CBE60, (void* self, void* , int target));

static char __stdcall GfxCollision_IntersectReferencedTriangle(
    std::uint32_t* query, float* line, float* intersection)
{
    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* scene = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(loader, nullptr));

    if (!query)
        return 0;

    const std::uint32_t referencedObject = CSF_AT(std::uint32_t, query, 0x34);
    if (!referencedObject ||
        !GfxScene_MatchesReferencedObject(
            scene, nullptr, static_cast<int>(referencedObject)))
        return 0;

    if (CSF_AT(std::uint32_t, query, 0x38) <
        CSF_AT(std::uint32_t, query, 0x3C))
    {
        alignas(8) std::uint8_t directionBytes[0x18];
        alignas(8) std::uint8_t startBytes[0xC];
        float* direction = reinterpret_cast<float*>(directionBytes);
        float* start = reinterpret_cast<float*>(startBytes);

        start[0] = line[0];
        start[1] = line[1];
        start[2] = line[2];
        direction[3] = line[3];
        direction[0] = direction[3] - start[0];
        direction[4] = line[4];
        direction[5] = line[5];

        float* vertex2 = reinterpret_cast<float*>(
            CSF_AT(std::uint32_t, query, 0x30));
        direction[1] = direction[4] - start[1];
        float* vertex0 = reinterpret_cast<float*>(
            CSF_AT(std::uint32_t, query, 0x28));
        float* vertex1 = reinterpret_cast<float*>(
            CSF_AT(std::uint32_t, query, 0x2C));
        direction[2] = direction[5] - start[2];

        if (RtIntersectionLineTriangle(
                start, direction, vertex0, vertex1, vertex2, intersection))
        {
            ++CSF_AT(std::uint32_t, query, 0x38);
            return 1;
        }
    }

    CSF_AT(std::uint32_t, query, 0x34) = 0;
    CSF_AT(std::uint32_t, query, 0x38) = 0xFFFFFFFF;
    return 0;
}

HOOK(0x006EBD10, GfxCollision_IntersectReferencedTriangle, "auto");
