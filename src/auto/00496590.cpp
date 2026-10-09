#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SpatialQuery_Destroy, 0x004013C0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(void*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, SpatialQuery_InitializeCandidates, 0x0044FAA0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiControl_QueryNearbyCandidates, 0x004520D0, (CGuiControl* self, void* , void* result, void* query, int context, int flags, float radius));
CSF_FUNC(int, __fastcall, CCharActionFsm_GetRecordByIndex, 0x00485940, (CCharActionFsm* self, void* , int index));
CSF_FUNC(int, __fastcall, CCharActionFsm_FindSubObjectIndex, 0x00485A20, (CCharActionFsm* self, void* , int targetValue));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Object_IsEligibleNearbyCandidate, 0x00492B90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(float*, __fastcall, EulerAnglesToDirection, 0x006AE640, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00833ee8_SelectNearbyActionTarget(
    C_00833ee8* self, void* /*edx*/)
{
    const int currentAction = static_cast<int>(self->field_0298);
    if (currentAction != 0)
        return currentAction;

    uint32_t candidates[11];
    SpatialQuery_InitializeCandidates(candidates, nullptr);

    int query[13] = {
        169, -1, -1, -1, 0, 0, 0, 10, 0, 0, 0, -1, 0
    };
    uint32_t context[6];
    float direction[3];
    float position[3];

    // The angle getter writes two floats across the original adjacent
    // angle and scratch slots; the scratch slot is subsequently reused.
    float angles[2] = {};
    float* angleResult =
        reinterpret_cast<float* (__fastcall*)(void*, void*, float*)>(
            reinterpret_cast<void**>(self->vtbl)[0xAC / 4])(
                self, nullptr, angles);
    float* facing = EulerAnglesToDirection(angleResult, nullptr, direction);

    void* positionResult =
        reinterpret_cast<void* (__fastcall*)(void*, void*, float*)>(
            reinterpret_cast<void**>(self->vtbl)[0xA4 / 4])(
                self, nullptr, position);
    SpatialQuery_BuildContext(
        context, nullptr, positionResult, reinterpret_cast<int>(facing));

    const int settings = GetGlobalValue_8BC1E0();
    int flags = 5;
    CGuiControl_QueryNearbyCandidates(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        candidates, query, reinterpret_cast<int>(context),
        reinterpret_cast<int>(&flags),
        CSF_AT(float, reinterpret_cast<void*>(settings), 0xB08));

    C_00833ee8* candidate =
        reinterpret_cast<C_00833ee8*>(candidates[0]);
    if (candidate && candidate->field_0050 == 1 &&
        static_cast<uint8_t>(
            Object_IsEligibleNearbyCandidate(candidate, nullptr)) != 0)
    {
        CCharActionFsm* actionFsm =
            reinterpret_cast<CCharActionFsm*>(self->field_0044);
        const int index =
            CCharActionFsm_FindSubObjectIndex(actionFsm, nullptr, 28);
        if (index != -1)
        {
            const int candidateValue =
                static_cast<int>(candidate->field_007c);
            const int record =
                CCharActionFsm_GetRecordByIndex(actionFsm, nullptr, index);
            if (candidateValue >
                CSF_AT(int, reinterpret_cast<void*>(record), 0x80))
            {
                float* targetPosition = reinterpret_cast<float*>(
                    GetObjectPositionVector(candidate, nullptr));
                void* ownPosition = reinterpret_cast<void*>(
                    GetObjectPositionVector(self, nullptr));
                float* displacement = BuildFacingVectorBetween(
                    ownPosition, nullptr, position, targetPosition);
                const float distance =
                    static_cast<float>(Vector_GetLength(displacement, nullptr));
                const int distanceSettings = GetGlobalValue_8BC1E0();

                if (distance < CSF_AT(
                        float, reinterpret_cast<void*>(distanceSettings), 0xB08))
                {
                    self->field_0298 = 5;
                    self->field_02a0 = candidate->field_0020;
                    self->field_02a4 = candidate->field_0024;
                    self->field_02ac = MenuExtras_ParseDescriptionId(
                        CSF_STR(0x00833E08, "G004"));
                }
            }
        }
        return SpatialQuery_Destroy(query, nullptr);
    }

    if (query[4] != 0)
        return Mem_Free(reinterpret_cast<void*>(query[4]));
    return 0;
}

HOOK(0x00496590, C_00833ee8_SelectNearbyActionTarget, "auto");
