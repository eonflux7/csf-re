#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(char, __stdcall, GfxCollision_IntersectReferencedTriangle, 0x006EBD10, (std::uint32_t* query, float* line, float* intersection));
CSF_FUNC(char, __fastcall, GfxCollision_QueryWorldIntersections, 0x006ECD80, (void* self, void* , int world, int queryData));
CSF_FUNC(char, __fastcall, GfxCollision_IntersectAtomic, 0x006ECDF0, (void* self, void* , int atomicArgument, int queryAddress));

static char __fastcall GfxCollision_QueryIntersection(
    void* self, void* /*edx*/, std::uint32_t* query, float* line,
    int* queryValue, std::uint32_t flags)
{
    void* scene = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    bool queryWorld = (flags & 1) != 0;
    bool queryAtomics = (flags & 2) != 0;
    char worldIntersection = 0;

    const unsigned worldShift =
        CSF_AT(std::uint8_t, self, 0x4) == 0 ? 4 : 2;
    const bool worldOverride = ((flags >> worldShift) & 1) != 0;
    const bool atomicOverride = ((flags >> (worldShift + 1)) & 1) != 0;
    if (worldOverride || atomicOverride)
    {
        queryWorld = worldOverride;
        queryAtomics = atomicOverride;
    }

    if (!queryWorld && !queryAtomics)
        return 0;

    alignas(8) std::uint8_t intersectionBytes[0xC];
    const char triangleIntersection = GfxCollision_IntersectReferencedTriangle(
        query, line, reinterpret_cast<float*>(intersectionBytes));
    if (triangleIntersection == 1)
        return triangleIntersection;

    alignas(8) std::uint8_t queryBytes[0x10];
    CSF_AT(std::uint8_t, queryBytes, 0x1) = (flags & 0x80) != 0;
    CSF_AT(int, queryBytes, 0x4) = *queryValue;
    CSF_AT(float*, queryBytes, 0x8) = line;
    const std::uint8_t mode = CSF_AT(std::uint8_t, self, 0x4);
    CSF_AT(std::uint8_t, queryBytes, 0x2) = 0;
    CSF_AT(std::uint8_t, queryBytes, 0x0) = mode;
    CSF_AT(std::uint8_t, queryBytes, 0x3) = (flags >> 8) & 1;
    CSF_AT(std::uint32_t*, queryBytes, 0xC) = query;

    if (queryWorld)
    {
        int world;
        if (mode)
        {
            void* reference =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, scene, 0)[0x40 / 4])(scene, nullptr);
            world = reinterpret_cast<int>(
                GetReferencedObject(reference, nullptr));
        }
        else
        {
            void* collection =
                ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, scene, 0)[0x44 / 4])(scene, nullptr);
            world = SceneLoader_GetWorldObjectCollection(collection, nullptr);
        }

        if (world)
            worldIntersection = GfxCollision_QueryWorldIntersections(
                self, nullptr, world, reinterpret_cast<int>(queryBytes));

        if (!worldIntersection)
        {
            int candidateWorld = 0;
            const std::uint32_t count =
                ((std::uint32_t (__fastcall*)(void*, void*))
                    CSF_AT(void**, scene, 0)[0x88 / 4])(scene, nullptr);

            for (std::uint32_t index = 0; index < count; ++index)
            {
                if (CSF_AT(std::uint8_t, self, 0x4))
                {
                    void* reference =
                        ((void* (__fastcall*)(void*, void*, std::uint32_t))
                            CSF_AT(void**, scene, 0)[0x8C / 4])(
                                scene, nullptr, index);
                    if (reference)
                        candidateWorld = reinterpret_cast<int>(
                            GetReferencedObject(reference, nullptr));
                }
                else
                {
                    void* collection =
                        ((void* (__fastcall*)(void*, void*, std::uint32_t))
                            CSF_AT(void**, scene, 0)[0x90 / 4])(
                                scene, nullptr, index);
                    if (collection)
                        candidateWorld =
                            SceneLoader_GetWorldObjectCollection(collection, nullptr);
                }

                if (candidateWorld)
                    worldIntersection = GfxCollision_QueryWorldIntersections(
                        self, nullptr, candidateWorld,
                        reinterpret_cast<int>(queryBytes));

                if (worldIntersection)
                    break;
            }
        }
    }

    if (queryAtomics && !worldIntersection &&
        CSF_AT(std::uint8_t, self, 0x4) &&
        CSF_AT(int, line, 0x18) == 1)
    {
        void* reference =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, scene, 0)[0x40 / 4])(scene, nullptr);
        if (reference)
        {
            const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x5C);
            for (std::uint32_t index = 0; index < count; ++index)
            {
                int* atomics = CSF_AT(int*, self, 0x54);
                if (GfxCollision_IntersectAtomic(
                        self, nullptr, atomics[index],
                        reinterpret_cast<int>(queryBytes)))
                    return CSF_AT(char, queryBytes, 0x2);
            }
        }
    }

    return worldIntersection;
}

HOOK(0x006ECE90, GfxCollision_QueryIntersection, "auto");
