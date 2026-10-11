#include <cstdint>
#include <cstddef>
#include <cfloat>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, SceneQueries_InitializeFilter, 0x004011F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpatialQuery_Destroy, 0x004013C0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, HudAlertIcons_BuildQueryContext, 0x004021C0, (void* self, void* edx, void* a2, void** a3));
CSF_FUNC(uint32_t*, __fastcall, InitializeNearbyCandidateResult, 0x0044FAA0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CGuiControl_FindNearbyChildPosition, 0x00450210, (CGuiControl* self, void* , int referenceObject, float maximumDistance, uint32_t* excludedFirst, uint32_t* excludedSecond, uint32_t* outputPosition));
CSF_FUNC(void, __fastcall, CGuiControl_QueryNearbyCandidates, 0x004520D0, (CGuiControl* self, void* , void* result, void* query, int context, int flags, float radius));
CSF_FUNC(char*, __fastcall, GetEmbeddedBlockAt74, 0x00452F10, (S_004503e0* self, void*));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(int, __fastcall, HudAlertIcons_UpdateCandidateIcon, 0x0049FA50, (void* self, void* edx, void* a2, void* a3, int a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

CSF_GLOBAL(float, g_LayoutWidthAdjustment, 0x00833E44);
CSF_GLOBAL(float, g_AlertCandidateMaximumDistanceSquared, 0x008343AC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* __fastcall S_004a00e0_UpdateVisibleAlertIcons(
    S_004a00e0* self, void* /*edx*/, void* source)
{
    if (!source || CSF_AT(int, source, 0x54) != 2)
        return source;

    int manager = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(manager), nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(firstElement), nullptr);
    if (!provider)
        return nullptr;

    char* candidates = GetEmbeddedBlockAt74(
        reinterpret_cast<S_004503e0*>(g_pCurrentScene), nullptr);
    uint32_t remaining = CSF_AT(uint32_t, candidates, 0x8);
    if (!remaining)
        return nullptr;

    alignas(8) uint8_t filterBytes[0x34];
    alignas(8) uint8_t candidatePositionBytes[0xC];
    alignas(8) uint8_t resultBytes[0x2C];
    alignas(8) uint8_t childPositionBytes[0xC];
    alignas(8) uint8_t contextBytes[0x1C] = {};
    alignas(8) uint8_t flagsBytes[0x8] = {};
    alignas(8) uint8_t displacementBytes[0x10] = {};
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t originBytes[0xC];

    float* displacement = reinterpret_cast<float*>(displacementBytes);
    float* direction = reinterpret_cast<float*>(directionBytes);
    float* origin = reinterpret_cast<float*>(originBytes);

    uint32_t entryOffset = 0;
    do
    {
        uint8_t* entry = CSF_AT(uint8_t*, candidates, 0) + entryOffset;
        if (CSF_AT(float, entry, 0x4) <= g_AlertCandidateMaximumDistanceSquared)
        {
            C_008342e8* candidate = CSF_AT(C_008342e8*, entry, 0);
            if (candidate &&
                (candidate->field_0170 & 0x2000) != 0 &&
                candidate->field_0078 == 1)
            {
                InitializeNearbyCandidateResult(resultBytes, nullptr);
                SceneQueries_InitializeFilter(filterBytes, nullptr);
                CSF_AT(uint32_t, filterBytes, 0) = 147;

                void* referenced = GetReferencedObject(provider, nullptr);
                void* transform =
                    ((void* (__fastcall*)(void*, void*))
                        CSF_AT(void**, referenced, 0)[0x10 / 4])
                    (referenced, nullptr);
                float* position =
                    ((float* (__fastcall*)(void*, void*))
                        CSF_AT(void**, transform, 0)[0x10 / 4])
                    (transform, nullptr);

                origin[0] = position[0];
                origin[1] = position[1];
                origin[2] = position[2];

                float* candidatePosition =
                    ((float* (__fastcall*)(void*, void*, void*))
                        candidate->vtbl[0xA4 / 4])
                    (candidate, nullptr, candidatePositionBytes);

                displacement[0] = candidatePosition[0] - origin[0];
                displacement[1] = candidatePosition[1] - origin[1];
                displacement[2] = candidatePosition[2] - origin[2];
                direction[0] = displacement[0];
                direction[1] = displacement[1];
                direction[2] = displacement[2];

                NormalizeVector3(
                    reinterpret_cast<C_00833b88*>(directionBytes), nullptr);

                direction[0] *= g_LayoutWidthAdjustment;
                CSF_AT(float, contextBytes, 0) = 1.0f;
                std::memset(contextBytes + 4, 0, 20);
                direction[1] *= g_LayoutWidthAdjustment;
                direction[2] *= g_LayoutWidthAdjustment;
                origin[0] += direction[0];
                origin[1] += direction[1];
                origin[2] += direction[2];

                HudAlertIcons_BuildQueryContext(
                    contextBytes, nullptr, originBytes,
                    reinterpret_cast<void**>(displacementBytes));

                CSF_AT(int, flagsBytes, 0) = 5;
                CGuiControl_QueryNearbyCandidates(
                    reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
                    resultBytes, filterBytes,
                    reinterpret_cast<int>(contextBytes),
                    reinterpret_cast<int>(flagsBytes), FLT_MAX);

                if (CSF_AT(C_008342e8*, resultBytes, 0x4) == candidate)
                {
                    std::memset(childPositionBytes, 0, sizeof(childPositionBytes));
                    if (!CGuiControl_FindNearbyChildPosition(
                            reinterpret_cast<CGuiControl*>(g_pCurrentScene),
                            nullptr, reinterpret_cast<int>(contextBytes),
                            CSF_AT(float, resultBytes, 0xC), nullptr, nullptr,
                            reinterpret_cast<uint32_t*>(childPositionBytes)))
                    {
                        int iconValue = C_004950e0_GetField398(source, nullptr);
                        HudAlertIcons_UpdateCandidateIcon(
                            self, nullptr, provider, candidate, iconValue);
                    }
                }

                SpatialQuery_Destroy(filterBytes, nullptr);
            }
        }
        entryOffset += 16;
        --remaining;
    } while (remaining);

    return nullptr;
}

HOOK(0x004A00E0, S_004a00e0_UpdateVisibleAlertIcons, "auto");
