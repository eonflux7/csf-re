#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalNestedField14, 0x0044F3D0, ());
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(const uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetPathPointRecord, 0x004A0F30, (C_0085e7b8* self, void* , int index));
CSF_FUNC(int, __fastcall, SceneFindSubObject, 0x004DE7A0, (void* self, void* , unsigned int a2));
CSF_FUNC(char, __fastcall, S_004dfbc0_FindPath, 0x004E06C0, (S_004dfbc0* self, void* , unsigned int a2, int a3, uint32_t* a4, char a5, int* a6, int a7));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AppendDistinctPoint, 0x006AF450, (void* self, void* , const void* point));
CSF_FUNC(int, __fastcall, S_006aef40_BuildDistanceTable, 0x006AF6E0, (S_006aef40* self, void* , int durationBits));
CSF_FUNC(char, __fastcall, S_0075f7c0_FindPath_75FC40, 0x0075FC40, (S_0075f7c0* self, void* , float* origin, float* destination, C_0085e7b8* path, uint32_t* destinationValue));

CSF_GLOBAL(float, g_MinMoveDistSq_82FCB4, 0x0082FCB4);
CSF_GLOBAL(uint32_t, g_PathBuffer, 0x00899504);
CSF_GLOBAL(uint32_t, g_PathCount, 0x0089950C);
CSF_GLOBAL(uint32_t, g_PathParam, 0x00899514);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CharacterMovement_SetDestination(
    char* self, void* /*edx*/, float* destination,
    S_006aef40* distanceParameters, int /*unused*/)
{
    CSF_AT(uint32_t, self, 0x18) &= 0xFFFFFFF9;

    const uint32_t* position = GetObjectPositionVector(
        reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14)), nullptr);
    CSF_AT(uint32_t, self, 0x38) = position[0];
    CSF_AT(uint32_t, self, 0x3C) = position[1];
    CSF_AT(uint32_t, self, 0x40) = position[2];

    float* origin = reinterpret_cast<float*>(self + 0x38);
    const float dx = origin[0] - destination[0];
    const float dy = origin[1] - destination[1];
    const float dz = origin[2] - destination[2];

    if (std::sqrt(dz * dz + dy * dy + dx * dx) <= g_MinMoveDistSq_82FCB4)
    {
        void* ownerBase = reinterpret_cast<void*>(
            CSF_AT(uint32_t, self, 0x14) + 4);
        CSF_AT(uint32_t, self, 0x20) = 0;
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, ownerBase, 0)[0x10 / 4])(ownerBase, nullptr);
        CSF_AT(uint32_t, self, 0x18) |= 4;
        return ((int(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, self, 0)[0xA8 / 4])(
                self, nullptr, CSF_AT(uint32_t, self, 0x44));
    }

    bool pathBuilt = false;
    if (CSF_AT(uint32_t, self, 0x1C) == 0)
    {
        auto* path = reinterpret_cast<C_0085e7b8*>(self + 0x24);
        uint32_t destinationValue = reinterpret_cast<uint32_t>(destination);
        auto* provider = reinterpret_cast<S_0075f7c0*>(GetGlobalNestedField14());

        if (S_0075f7c0_FindPath_75FC40(
                provider, nullptr, origin, destination, path, &destinationValue)
            && GetCachedObjectProvider(path, nullptr))
        {
            uint32_t points = ((uint32_t(__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
            CSF_AT(uint32_t, self, 0x44) = points;
            AppendDistinctPoint(reinterpret_cast<void*>(points + 4), nullptr, origin);

            for (uint32_t index = 0;
                 index < reinterpret_cast<uint32_t>(GetCachedObjectProvider(path, nullptr));
                 ++index)
            {
                int point = C_0085e7b8_GetPathPointRecord(
                    path, nullptr, static_cast<int>(index));
                AppendDistinctPoint(
                    reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x44) + 4),
                    nullptr, reinterpret_cast<const void*>(point));
            }
            pathBuilt = true;
        }
    }
    else
    {
        void* scenePaths = reinterpret_cast<void*>(CGuiControl_GetField012C(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr));

        if (SceneFindSubObject(scenePaths, nullptr, reinterpret_cast<uint32_t>(origin)))
        {
            float* requestedDestination = destination;
            scenePaths = reinterpret_cast<void*>(CGuiControl_GetField012C(
                reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr));

            if (SceneFindSubObject(
                    scenePaths, nullptr, reinterpret_cast<uint32_t>(requestedDestination)))
            {
                g_PathCount = 0;
                g_PathParam = static_cast<uint32_t>(-1082130432);
                scenePaths = reinterpret_cast<void*>(CGuiControl_GetField012C(
                    reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr));
                auto* pathFinder = reinterpret_cast<S_004dfbc0*>(
                    GetCurrentItemCount(scenePaths, nullptr));

                if (S_004dfbc0_FindPath(
                        pathFinder, nullptr, reinterpret_cast<uint32_t>(origin),
                        reinterpret_cast<int>(requestedDestination),
                        &g_PathBuffer, 0, nullptr, 0)
                    && g_PathCount)
                {
                    CSF_AT(uint32_t, self, 0x44) =
                        ((uint32_t(__fastcall*)(void*, void*))
                            CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);

                    uint32_t remaining = g_PathCount;
                    uint32_t offset = 0;
                    while (remaining)
                    {
                        const void* point =
                            reinterpret_cast<const void*>(g_PathBuffer + offset + 4);
                        AppendDistinctPoint(
                            reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x44) + 4),
                            nullptr, point);
                        offset += 20;
                        --remaining;
                    }
                    pathBuilt = true;
                }
            }
        }
    }

    if (pathBuilt)
    {
        S_006aef40_BuildDistanceTable(
            reinterpret_cast<S_006aef40*>(CSF_AT(uint32_t, self, 0x44) + 4),
            nullptr, reinterpret_cast<int>(distanceParameters));
        CSF_AT(uint32_t, self, 0x20) = 1;
        uint32_t flags = CSF_AT(uint32_t, self, 0x18) | 2;
        CSF_AT(uint32_t, self, 0x18) = flags;
        return static_cast<int>(flags);
    }

    uint32_t points = CSF_AT(uint32_t, self, 0x44);
    auto releasePath = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, self, 0)[0xA8 / 4]);
    CSF_AT(uint32_t, self, 0x20) = 0;
    return releasePath(self, nullptr, points);
}

HOOK(0x00425200, CharacterMovement_SetDestination, "auto");
