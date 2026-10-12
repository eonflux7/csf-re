#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_FindNearbyChildPosition, 0x00450210, (CGuiControl* self, void* , int referenceObject, float maximumDistance, uint32_t* excludedFirst, uint32_t* excludedSecond, uint32_t* outputPosition));
CSF_FUNC(float*, __fastcall, StaticAi_SetProbeSegment, 0x004A9FB0, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(uint32_t*, __fastcall, StaticAi_InitializeWaypointAssessment, 0x004ADFC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, StaticAi_TestLinkedObstruction, 0x004AF450, (void* self, void* edx, float* a3, uint32_t* a6));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, StaticAi_GetCachedWaypointAssessment, 0x004DC980, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, StaticAi_StoreWaypointAssessment, 0x004DD110, (void* self, void* edx, uint32_t* a2, void* a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_SetYRotation, 0x006AE2A0, (S_006ada10* self, void* , float angle));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, C_0083e0d8_FindNearestWaypoint, 0x0075CB30, (C_0083e0d8* self, void* , int groupId, float* position, uint32_t* result));
CSF_FUNC(double, __fastcall, C_0086c288_GetFacingAngleRadians, 0x0075D0A0, (C_0086c288* self, void* , uint32_t* a2));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_CharacterFacingDistance, 0x00833854);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_004af750_EvaluateGuardWaypoint(
    S_004af750* self, void* /*edx*/, float* position)
{
    void* gameState = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetValue8B0554()));
    void* waypointManager = CSF_AT(void*, gameState, 0x1FD8);

    alignas(8) uint8_t waypointBytes[0x8];
    auto* waypoint = reinterpret_cast<uint32_t*>(waypointBytes);
    waypoint[0] = static_cast<uint32_t>(-1);
    waypoint[1] = static_cast<uint32_t>(-1);

    if (!C_0083e0d8_FindNearestWaypoint(
            reinterpret_cast<C_0083e0d8*>(waypointManager),
            nullptr, 3, position, waypoint))
        return 0;

    int cachedResult = StaticAi_GetCachedWaypointAssessment(
        waypointManager, nullptr, waypoint);
    if (cachedResult)
        return cachedResult;

    alignas(8) uint8_t assessmentBytes[0x8C];
    // These two packet annotations identify call targets, not vtable stores.
    (void)CSF_VTBL(0x004ADFC0);
    (void)CSF_VTBL(0x004AF450);
    StaticAi_InitializeWaypointAssessment(assessmentBytes, nullptr);

    void* sceneLoader = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalInt()));
    void* sceneController = reinterpret_cast<void*>(
        static_cast<uintptr_t>(
            SceneLoader_GetSceneController(sceneLoader, nullptr)));

    float facingAngle = static_cast<float>(
        C_0086c288_GetFacingAngleRadians(
            reinterpret_cast<C_0086c288*>(waypointManager),
            nullptr, waypoint));

    alignas(8) uint8_t matrixBytes[0x8];
    S_006ada10& rotation = *reinterpret_cast<S_006ada10*>(matrixBytes);
    rotation.field_0000 = reinterpret_cast<uintptr_t>(RwMatrixCreate());
    CSF_AT(uint8_t, matrixBytes, 0x4) = 1;
    GfxMatrix_SetYRotation(&rotation, nullptr, facingAngle);

    auto* matrix = reinterpret_cast<uint8_t*>(rotation.field_0000);
    auto* forwardAxis = matrix + 0x20;
    auto* samples = reinterpret_cast<float*>(assessmentBytes + 0x50);

    float raisedY = position[1] + g_CharacterFacingDistance;
    float initialX = position[0];
    float sideX = CSF_AT(float, matrix, 0x0) * g_MaxPercent;
    float initialZ = position[2];
    samples[0] = initialX;
    float sideY = CSF_AT(float, matrix, 0x4) * g_MaxPercent;
    samples[1] = raisedY;
    samples[2] = initialZ;
    float sideZ = CSF_AT(float, matrix, 0x8) * g_MaxPercent;

    samples[3] = sideX + position[0];
    samples[4] = (sideY + position[1]) + g_fReturnedConstant_0082E030;
    samples[5] = sideZ + position[2];

    samples[6] = sideX + position[0];
    samples[7] = (sideY + position[1]) + g_CharacterFacingDistance;
    samples[8] = sideZ + position[2];

    void* linkedMember = reinterpret_cast<uint8_t*>(self) + 0x8;
    alignas(8) uint8_t referenceBytes[0x18];
    CSF_AT(uint32_t, referenceBytes, 0x0) = 1065353216;
    std::memset(referenceBytes + 0x4, 0, 20);

    samples[9] = position[0] - sideX;
    samples[10] = (position[1] - sideY) + g_fReturnedConstant_0082E030;
    samples[11] = position[2] - sideZ;

    samples[12] = position[0] - sideX;
    samples[13] = (position[1] - sideY) + g_CharacterFacingDistance;
    samples[14] = position[2] - sideZ;

    // The two adjacent three-float locals form the segment passed to the tests.
    alignas(8) uint8_t segmentBytes[0x18];
    auto* segmentStart = reinterpret_cast<float*>(segmentBytes);
    auto* segmentEnd = reinterpret_cast<float*>(segmentBytes + 0xC);

    alignas(8) uint8_t outputBytes[0x18];
    auto* outputPosition = reinterpret_cast<uint32_t*>(outputBytes);
    // The upper half was also used as a vector scratch local in the original.
    CSF_AT(float, outputBytes, 0xC) = samples[12];
    CSF_AT(float, outputBytes, 0x10) = samples[13];
    CSF_AT(float, outputBytes, 0x14) = samples[14];

    alignas(8) uint8_t collisionFlagsBytes[0x20];
    auto* collisionFlags = reinterpret_cast<int*>(collisionFlagsBytes);

    uint32_t sampleOffset = 0;
    uint32_t statusOffset = 0;
    do
    {
        auto* sample = reinterpret_cast<float*>(
            reinterpret_cast<uint8_t*>(samples) + sampleOffset);

        CSF_AT(int, assessmentBytes, statusOffset) = 0;
        CSF_AT(float, assessmentBytes, 0x14 + sampleOffset) = sample[0];
        CSF_AT(float, assessmentBytes, 0x18 + sampleOffset) = sample[1];
        CSF_AT(float, assessmentBytes, 0x1C + sampleOffset) = sample[2];

        segmentStart[0] = position[0];
        segmentStart[1] = position[1];
        segmentStart[2] = position[2];
        segmentEnd[0] = sample[0];
        segmentEnd[1] = sample[1];
        segmentEnd[2] = sample[2];

        outputPosition[0] = 0;
        CSF_AT(float, outputBytes, 0x4) = 0.0f;
        CSF_AT(float, outputBytes, 0x8) = 0.0f;

        int linkedValue = GetLinkedObjectValue_4DB5C0(linkedMember, nullptr);
        void* linkedObject = linkedValue
            ? reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue) - 4)
            : nullptr;

        if (!StaticAi_TestLinkedObstruction(
                linkedObject, nullptr, segmentStart, outputPosition))
        {
            CSF_AT(int, assessmentBytes, statusOffset) = 1;
            float forwardX = CSF_AT(float, forwardAxis, 0x0) * g_MaxPercent;
            float forwardY = CSF_AT(float, forwardAxis, 0x4) * g_MaxPercent;
            float forwardZ = CSF_AT(float, forwardAxis, 0x8) * g_MaxPercent;

            segmentStart[0] = sample[0];
            segmentStart[1] = sample[1];
            segmentStart[2] = sample[2];

            CSF_AT(float, outputBytes, 0xC) = forwardX + sample[0];
            segmentEnd[0] = CSF_AT(float, outputBytes, 0xC);
            CSF_AT(float, outputBytes, 0x10) = forwardY + sample[1];
            segmentEnd[1] = CSF_AT(float, outputBytes, 0x10);
            CSF_AT(float, outputBytes, 0x14) = forwardZ + sample[2];
            segmentEnd[2] = CSF_AT(float, outputBytes, 0x14);

            int collisionObjectValue =
                reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, sceneController, 0x0)[60 / 4])(
                        sceneController, nullptr);
            void* collisionObject = reinterpret_cast<void*>(
                static_cast<uintptr_t>(collisionObjectValue));
            *collisionFlags = 1;

            int obstructed =
                reinterpret_cast<int (__fastcall*)(
                    void*, void*, int, float*, int*, int)>(
                        CSF_AT(void**, collisionObject, 0x0)[52 / 4])(
                            collisionObject, nullptr, 0,
                            segmentStart, collisionFlags, 3);

            if (!obstructed)
            {
                StaticAi_SetProbeSegment(
                    referenceBytes, nullptr, segmentStart, segmentEnd);
                if (!CGuiControl_FindNearbyChildPosition(
                        reinterpret_cast<CGuiControl*>(g_pCurrentScene),
                        nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(referenceBytes)),
                        100.0f, nullptr, nullptr, outputPosition))
                {
                    CSF_AT(int, assessmentBytes, statusOffset) = 2;
                }
            }
        }

        statusOffset += 4;
        sampleOffset += 12;
    } while (sampleOffset < 0x3C);

    void* result = StaticAi_StoreWaypointAssessment(
        waypointManager, nullptr, waypoint, assessmentBytes);

    if (CSF_AT(uint8_t, matrixBytes, 0x4))
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(rotation.field_0000));

    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x004AF750, S_004af750_EvaluateGuardWaypoint, "auto");
