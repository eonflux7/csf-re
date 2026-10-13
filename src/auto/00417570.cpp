#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_00405350_ForwardWithDefaultFlags, 0x00405350, (S_00405180* self, void* , float* a2, float* a3, int* a4));
CSF_FUNC(char, __fastcall, S_00405180_TraceGroundWithState, 0x004053A0, (S_00405180* self, void* , float* a2, float* a3, int* a4));
CSF_FUNC(void*, __fastcall, SpatialQuery_InitializeQueryInfo, 0x0040E070, (void* self, void*));
CSF_FUNC(C_00833b88*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (C_00833b88* self, void* , uint32_t* a2, uint32_t* a3));
CSF_FUNC(void, __fastcall, C_0082e090_DampenOnceIfIdle, 0x00415E10, (C_0082e090* self, void*));
CSF_FUNC(void, __fastcall, C_0082e090_ApplyRotationInput, 0x00415E70, (C_0082e090* self, void* , float amount, S_00526d40* direction));
CSF_FUNC(uint32_t*, __fastcall, C_0082e090_ResolveLinkedChild, 0x00416220, (C_0082e090* self, void*));
CSF_FUNC(char, __fastcall, C_0082e090_UpdateVerticalSpeedWithGravity, 0x00416280, (C_0082e090* self, void* , float elapsedSeconds));
CSF_FUNC(uint32_t*, __fastcall, C_0082e0f4_InitializeSpawnRecord, 0x004164B0, (C_0082e0f4* self, void* , int ownerId));
CSF_FUNC(void, __fastcall, C_0082e090_HandleCollision, 0x00416AE0, (C_0082e090* self, void* , const float* displacement, uint32_t* collision, int));
CSF_FUNC(void, __fastcall, C_0082e090_HandleCollisionModeTwo, 0x00416D10, (void* self, void* edx, int a2, float a3, int a4));
CSF_FUNC(void, __fastcall, C_0082e090_HandleCollisionModeFive, 0x00416DE0, (void* self, void* edx, int a2, float a3, int a4));
CSF_FUNC(int, __fastcall, C_0082e090_HandleSpecialCollisionObject, 0x00416EA0, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(void, __fastcall, C_0082e090_HandleCollisionModeFour, 0x00417020, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, C_0082e520_UpdateFromEvent, 0x0041E090, (C_0082e520* self, void* , int));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, C_008342e8_GetDrawState, 0x0049CF10, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, S_0049cfa0_SyncPositionAndNotifyChild, 0x0049CFA0, (S_0049cfa0* self, void* , uint32_t* a2));
CSF_FUNC(int, __fastcall, SetFieldAt1B4, 0x0049D090, (void* self, void* , int value));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, TextDatabase_FindSceneEntryAtPosition, 0x004EF6E0, (void* self, void* , S_00770760* position, char category));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(int, __fastcall, GfxMatrix_ClassifyPointAgainstPlane, 0x006AD7B0, (float* self, void* , float* point));
CSF_FUNC(void*, __fastcall, ResetEmbeddedMemberAt8, 0x00712D10, (void** self, void* edx));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_MessageBoxCollapsedScale, 0x0082DFCC);
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);
CSF_GLOBAL(double, g_MovementEpsilon, 0x0082E078);
CSF_GLOBAL(double, g_GroundTraceInterval, 0x0082E080);
CSF_GLOBAL(float, g_Const20, 0x0082E0F0);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The packet also classifies the notify call's address as a vtable reference.
static auto const kReportedNotifyVtable = CSF_VTBL(0x0049CFA0);

static void __fastcall C_0082e090_UpdateMovementState(
    C_0082e090* self, void* /*edx*/, int eventBits)
{
    if (!self->field_000c)
        return;

    C_0082e520_UpdateFromEvent(self, nullptr, eventBits);
    if (!self->field_0077)
        return;

    float elapsed;
    std::memcpy(&elapsed, &eventBits, sizeof(elapsed));

    // Preserve the complete frame-sized workspace: several apparent locals
    // are parts of larger records written by the callees.
    alignas(8) uint8_t workspace[0x128]{};
    auto& record = *reinterpret_cast<C_0082e0f4*>(workspace);
    auto* query = workspace + 0x74;
    auto* contactPoint = reinterpret_cast<float*>(workspace + 0x7C);
    auto* plane = reinterpret_cast<float*>(workspace + 0x94);
    auto* normal = reinterpret_cast<float*>(workspace + 0xA0);
    auto& collisionObject = CSF_AT(uint32_t*, workspace, 0xC0);
    auto* queryResult = workspace + 0xD8;
    auto* convertedDirection = reinterpret_cast<uint32_t*>(workspace + 0xE0);
    auto* movement = reinterpret_cast<float*>(workspace + 0xF0);
    auto* displacement = reinterpret_cast<float*>(workspace + 0x100);
    auto* context = reinterpret_cast<C_00833b88*>(workspace + 0x10C);
    auto* position = reinterpret_cast<float*>(workspace + 0x118);
    auto& traceState = CSF_AT(int, workspace, 0x124);

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* body =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* originalOwner = reinterpret_cast<void*>(self->field_0004);

    if (self->field_0090)
    {
        self->field_0088 = -100.0f;
    }
    else
    {
        void* sceneEntries = CSF_AT(
            void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FDC);
        auto* ownerPosition = reinterpret_cast<S_00770760*>(
            GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0004), nullptr));

        if (TextDatabase_FindSceneEntryAtPosition(
                sceneEntries, nullptr, ownerPosition, 1) == -1)
        {
            C_0082e090_UpdateVerticalSpeedWithGravity(self, nullptr, elapsed);
        }
        else
        {
            void* currentOwner = reinterpret_cast<void*>(self->field_0004);
            self->field_0090 = 1;
            int ownerType = CSF_AT(int, currentOwner, 0x50);
            if (ownerType != 21 && ownerType != 20)
            {
                void* recordManager = CSF_AT(
                    void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FD4);
                C_0082e0f4_InitializeSpawnRecord(&record, nullptr, 4);

                int globalValue = GetGlobalValue_8BC1E0();
                record.field_005c =
                    reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
                        CSF_AT(void**, recordManager, 0)[0x10 / 4])(
                        recordManager, nullptr,
                        CSF_AT(uint32_t, reinterpret_cast<void*>(globalValue), 0x904));

                uint32_t* direction = S_00526d40_ConvertDirection(
                    reinterpret_cast<S_00526d40*>(&self->field_0084),
                    nullptr, convertedDirection);
                record.field_004c = direction[0];
                record.field_0050 = direction[1];
                record.field_0054 = direction[2];

                uint32_t* sourcePosition =
                    GetObjectPositionVector(originalOwner, nullptr);
                record.field_0034 = sourcePosition[0];
                record.field_0038 = sourcePosition[1];
                uint32_t z = sourcePosition[2];
                CSF_AT(float, &record, 0x40) = g_AxisY;
                uint32_t ownerValue = CSF_AT(uint32_t, originalOwner, 0x194);
                record.field_003c = z;
                record.field_0024 = ownerValue;
                CSF_AT(float, &record, 0x44) = g_AxisY_Y;
                CSF_AT(float, &record, 0x48) = g_AxisY_Z;

                record.field_0060 = CSF_AT(
                    uint32_t,
                    reinterpret_cast<void*>(C_008342e8_GetDrawState(
                        reinterpret_cast<C_008342e8*>(originalOwner), nullptr)),
                    0x70);
                record.field_006c = CSF_AT(
                    uint32_t,
                    reinterpret_cast<void*>(C_008342e8_GetDrawState(
                        reinterpret_cast<C_008342e8*>(originalOwner), nullptr)),
                    0x114);
                record.field_0070 = 0;

                int dispatcher = GetGlobalObjectField184();
                DispatchRecordToGlobalObject(
                    reinterpret_cast<void*>(dispatcher), nullptr, &record, 1);
            }

            position[0] = 0.0f;
            self->field_0084 = 0.0f;
            position[1] = elapsed * g_MaxPercent; // 100.0
            position[2] = 0.0f;
            self->field_0088 = position[1];
            self->field_008c = 0.0f;
        }
    }

    displacement[0] = elapsed * self->field_0084;
    movement[0] = displacement[0];
    displacement[1] = elapsed * self->field_0088;
    movement[1] = displacement[1];
    displacement[2] = elapsed * self->field_008c;
    movement[2] = displacement[2];
    self->field_00a8 = elapsed + self->field_00a8;
    self->field_00b8 = self->field_00b8 - elapsed;

    SpatialQuery_InitializeQueryInfo(query, nullptr);
    char hit;
    if (self->field_0090)
    {
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 1;
        traceState = 2;
        hit = S_00405350_ForwardWithDefaultFlags(
            reinterpret_cast<S_00405180*>(self->field_0008), nullptr,
            displacement, reinterpret_cast<float*>(query), &traceState);
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 0;
    }
    else
    {
        hit = self->vSlot54(displacement, reinterpret_cast<uint32_t*>(query));
    }

    if (hit &&
        normal[1] * self->field_0088 +
        normal[0] * self->field_0084 +
        normal[2] * self->field_008c < g_fZero)
    {
        if (GfxMatrix_ClassifyPointAgainstPlane(plane, nullptr, contactPoint))
        {
            CSF_AT(float, queryResult, 0x10) = g_AxisY_Z * g_LevelScaleFactor;
            position[0] = g_AxisY * g_LevelScaleFactor + contactPoint[0];
            contactPoint[0] = position[0];
            position[1] = contactPoint[1] + g_AxisY_Y * g_LevelScaleFactor;
            contactPoint[1] = position[1];
            position[2] = contactPoint[2] + CSF_AT(float, queryResult, 0x10);
            contactPoint[2] = position[2];
        }

        void* dispatched = C_0082dd78_DispatchReturnedObject(self, nullptr);
        void* transform =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, dispatched, 0)[0x34 / 4])(dispatched, nullptr);
        float* origin =
            reinterpret_cast<float*(__fastcall*)(void*, void*, uint32_t*)>(
                CSF_AT(void**, transform, 0)[0x1C / 4])(
                transform, nullptr, &record.field_0038);

        float dx = contactPoint[0] - origin[0];
        float dy = contactPoint[1] - origin[1];
        CSF_AT(float, queryResult, 0x10) = contactPoint[2] - origin[2];
        position[0] = dx * elapsed;
        movement[0] = position[0];
        position[1] = dy * elapsed;
        movement[1] = position[1];
        position[2] = CSF_AT(float, queryResult, 0x10) * elapsed;
        movement[2] = position[2];

        C_0082e090_DampenOnceIfIdle(self, nullptr);
        float actualVertical = std::fabs(movement[1]);
        float requestedVertical = std::fabs(displacement[1]);
        if (self->field_0088 < 0.0f &&
            requestedVertical > 0.000001 &&
            actualVertical < static_cast<double>(requestedVertical))
        {
            self->field_0088 = self->field_0088 -
                (requestedVertical - actualVertical) / requestedVertical *
                elapsed * -980.0f;
        }

        uint8_t specialMode = self->field_0090;
        self->field_0098 = self->field_0094;
        if (specialMode)
        {
            self->field_0077 = 0;
        }
        else
        {
            int resolvedCollisionObject = 0;
            uint32_t* direction = S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(displacement),
                nullptr, convertedDirection);
            uint32_t* ownerPosition = GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0004), nullptr);
            SpatialQuery_BuildContext(context, nullptr, ownerPosition, direction);

            if (collisionObject)
            {
                void* collisionDispatcher = C_0082dd78_DispatchReturnedObject(
                    reinterpret_cast<void*>(collisionObject[19]), nullptr);
                std::memset(queryResult, 0, 12);
                CSF_AT(uint8_t, queryResult, 0x10) = 0;
                CSF_AT(uint32_t, queryResult, 0xC) = 10;

                if (reinterpret_cast<bool(__fastcall*)(void*, void*, void*, void*, int)>(
                        CSF_AT(void**, collisionDispatcher, 0)[0x40 / 4])(
                        collisionDispatcher, nullptr, context, queryResult, 0))
                {
                    resolvedCollisionObject = CSF_AT(
                        int, CSF_AT(void*, queryResult, 0), 0x18);
                    SetFieldAt1B4(
                        reinterpret_cast<void*>(self->field_0004),
                        nullptr, resolvedCollisionObject);
                }

                ResetEmbeddedMemberAt8(
                    reinterpret_cast<void**>(queryResult), nullptr);

                if (collisionObject && collisionObject[20] == 4 &&
                    CSF_AT(int,
                        reinterpret_cast<void*>(C_008342e8_GetDrawState(
                            reinterpret_cast<C_008342e8*>(originalOwner), nullptr)),
                        0x70) == 8 &&
                    resolvedCollisionObject)
                {
                    C_0082e090_HandleSpecialCollisionObject(
                        self, nullptr, collisionObject, resolvedCollisionObject);
                }
            }

            // These legacy signatures represent pointer arguments as float bits.
            uint32_t queryAddress = reinterpret_cast<uint32_t>(query);
            float queryAddressBits;
            std::memcpy(&queryAddressBits, &queryAddress, sizeof(queryAddressBits));
            int displacementAddress = reinterpret_cast<int>(displacement);
            switch (self->field_0094)
            {
            case 1:
                C_0082e090_HandleCollision(
                    self, nullptr, displacement,
                    reinterpret_cast<uint32_t*>(query), eventBits);
                break;
            case 2:
                C_0082e090_HandleCollisionModeTwo(
                    self, nullptr, displacementAddress, queryAddressBits, eventBits);
                break;
            case 3:
                self->vSlot58(displacementAddress, query, eventBits);
                break;
            case 4:
                C_0082e090_HandleCollisionModeFour(
                    self, nullptr, displacementAddress,
                    reinterpret_cast<int>(query), eventBits);
                break;
            case 5:
                C_0082e090_HandleCollisionModeFive(
                    self, nullptr, displacementAddress, queryAddressBits, eventBits);
                break;
            default:
                break;
            }
        }
    }
    else
    {
        float* ownerPosition = reinterpret_cast<float*>(GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0004), nullptr));
        position[0] = ownerPosition[0];
        position[1] = ownerPosition[1];
        float z = ownerPosition[2];
        position[0] += displacement[0];
        position[2] = z;
        void* currentOwner = reinterpret_cast<void*>(self->field_0004);
        position[1] += displacement[1];
        position[2] = z + displacement[2];
        SceneLoader_ConfigureFocusEntry(
            currentOwner, nullptr, reinterpret_cast<int*>(position));
        void* dispatched = C_0082dd78_DispatchReturnedObject(self, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, dispatched, 0)[0x28 / 4])(dispatched, nullptr);
    }

    self->field_0080 = std::sqrt(
        movement[0] * movement[0] +
        movement[2] * movement[2] +
        movement[1] * movement[1]) + self->field_0080;

    if (elapsed * g_Const20 <= std::sqrt(
            self->field_0084 * self->field_0084 +
            self->field_0088 * self->field_0088 +
            self->field_008c * self->field_008c) * elapsed)
    {
        int remaining = static_cast<int32_t>(self->field_007c);
        if (remaining > 0)
        {
            self->field_007c = remaining - 1;
            if (!self->field_007c)
            {
                self->field_00b0 = -1;
                self->field_00b4 = -1;
            }
        }
    }
    else
    {
        uint32_t* collided = collisionObject;
        if (collided)
        {
            self->field_00b0 = collided[8];
            self->field_00b4 = collided[9];
        }

        if (std::fabs(self->field_00b8) >= 0.050000001)
        {
            self->field_007c += 3;
        }
        else
        {
            SpatialQuery_InitializeQueryInfo(&record.field_0010, nullptr);
            CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 1;
            traceState = 2;
            position[0] = 0.0f;
            position[1] = -5.0f;
            position[2] = 0.0f;
            char groundHit = S_00405180_TraceGroundWithState(
                reinterpret_cast<S_00405180*>(self->field_0008), nullptr,
                position, reinterpret_cast<float*>(&record.field_0010),
                &traceState);
            CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x5) = 0;
            if (groundHit)
                self->field_007c += 1;
        }

        if (static_cast<int32_t>(self->field_007c) >= 4)
            self->field_0077 = 0;
    }

    if (self->field_00a8 > 10.0f)
        self->field_0077 = 0;

    if (self->field_0077)
    {
        C_0082e090_ApplyRotationInput(
            self, nullptr, elapsed, reinterpret_cast<S_00526d40*>(movement));
    }
    else
    {
        C_0082e090_ResolveLinkedChild(self, nullptr);
        if (!(self->field_00b4 & 0x80000000) && self->field_0094 != 2)
        {
            int linkedAddress = CHandleTable_Resolve(
                g_pCurrentScene, nullptr, CSF_AT(int, self, 0xAC));
            auto* linked = reinterpret_cast<uint32_t*>(linkedAddress);
            if (linked)
            {
                int linkedType = CSF_AT(int, linked, 0x50);
                if ((static_cast<uint8_t>(
                         GameConfig_IsResolvedEntryActive(linkedType)) ||
                     linkedType == 12) &&
                    !reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, body, 0)[0xA8 / 4])(body, nullptr))
                {
                    void* linkedBody =
                        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, linked, 0)[0x28 / 4])(linked, nullptr);
                    if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, body, 0)[0x78 / 4])(body, nullptr) &&
                        linkedBody &&
                        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, linkedBody, 0)[0x78 / 4])(
                            linkedBody, nullptr))
                    {
                        void* bodyState =
                            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                                CSF_AT(void**, body, 0)[0x78 / 4])(body, nullptr);
                        reinterpret_cast<void(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, bodyState, 0)[0x20 / 4])(
                            bodyState, nullptr);
                    }
                    else
                    {
                        void* currentOwner =
                            reinterpret_cast<void*>(self->field_0004);
                        float x = normal[0] + contactPoint[0];
                        self->field_0077 = 0;
                        position[0] = x;
                        position[1] = normal[1] + contactPoint[1];
                        position[2] = normal[2] + contactPoint[2];
                        SceneLoader_ConfigureFocusEntry(
                            currentOwner, nullptr, reinterpret_cast<int*>(position));
                        void* dispatched =
                            C_0082dd78_DispatchReturnedObject(self, nullptr);
                        reinterpret_cast<void(__fastcall*)(void*, void*)>(
                            CSF_AT(void**, dispatched, 0)[0x28 / 4])(
                            dispatched, nullptr);
                        if (!CSF_AT(uint8_t,
                                reinterpret_cast<void*>(GetGlobalState_50D190()),
                                0xD0))
                        {
                            S_0049cfa0_SyncPositionAndNotifyChild(
                                reinterpret_cast<S_0049cfa0*>(self->field_0004),
                                nullptr, linked);
                        }
                    }
                }
            }
        }
    }
}

HOOK(0x00417570, C_0082e090_UpdateMovementState, "auto");
