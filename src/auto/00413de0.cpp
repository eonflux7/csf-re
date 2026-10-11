#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_405370, 0x00405370, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(char, __fastcall, sub_4053A0, 0x004053A0, (void* self, void* edx, float* a2, float* a3, int* a4));
CSF_FUNC(float*, __fastcall, sub_411EF0, 0x00411EF0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, sub_412400, 0x00412400, (void* self, void* edx, float a2, int a3, char a4));
CSF_FUNC(void, __fastcall, sub_412EE0, 0x00412EE0, (void* self, void* edx, float a2, int a3));
CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_413C60, 0x00413C60, (void* self, void* edx, int a2, float a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(uint32_t*, __fastcall, sub_4460A0, 0x004460A0, (void* self, void* edx, int a2, int a3, uint32_t* a4, int a5));
CSF_FUNC(unsigned int*, __fastcall, SceneRequest_Submit, 0x00446F10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(float*, __fastcall, S_00526d40_ProjectPointToPlane, 0x006AD830, (S_00526d40* self, void* , float* a2, float* a3));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_MovementLimit, 0x0082DFB8);
CSF_GLOBAL(float, g_ScaleY_0_6, 0x0082E038);
CSF_GLOBAL(float, g_FloatConstant_82e05c, 0x0082E05C);
CSF_GLOBAL(float, g_CharacterWidthScale, 0x0082E060);
CSF_GLOBAL(float, g_MovementProbeScale, 0x0082E064);
CSF_GLOBAL(float, g_Const_0082E068, 0x0082E068);
CSF_GLOBAL(float, g_SupportRangeScale, 0x0082E06C);
CSF_GLOBAL(float, g_GravityScale, 0x0082E070);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static void* Movement_Object(C_0082dfd0* self)
{
    return reinterpret_cast<void*>(self->field_0004);
}

static float* Movement_Position(void* object)
{
    return reinterpret_cast<float*>(GetObjectPositionVector(object, nullptr));
}

static void Movement_CopyVector(float* destination, const float* source)
{
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
}

static float Movement_LengthSquared(const float* vector)
{
    return vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
}

// The original computes its square roots inline (fsqrt). The hook DLL has no
// math library to call, so the root is evaluated here with Newton's method,
// starting from a guess that is never below the root and descending to it.
static double Movement_SquareRoot(double value)
{
    if (!(value > 0.0))
        return value;
    double root = value >= 1.0 ? value : 1.0;
    for (int iteration = 0; iteration < 128; ++iteration) {
        double next = 0.5 * (root + value / root);
        if (next >= root)
            break;
        root = next;
    }
    return root;
}

static float* Movement_UnitDirection(float* vector, float* output)
{
    return reinterpret_cast<float*>(S_00526d40_ConvertDirection(
        reinterpret_cast<S_00526d40*>(vector), nullptr,
        reinterpret_cast<uint32_t*>(output)));
}

static void Movement_SetPosition(C_0082dfd0* self, float* position)
{
    SceneLoader_ConfigureFocusEntry(Movement_Object(self), nullptr,
                                   reinterpret_cast<int*>(position));
}

static void Movement_RefreshShape(C_0082dfd0* self)
{
    void* shape = reinterpret_cast<void*>(C_0082dd78_DispatchReturnedObject(
        reinterpret_cast<C_0082dd78*>(self), nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, shape, 0)[0x28 / 4])(shape, nullptr);
}

static float* Movement_GetDimensions(C_0082dfd0* self)
{
    void* shape = reinterpret_cast<void*>(C_0082dd78_DispatchReturnedObject(
        reinterpret_cast<C_0082dd78*>(self), nullptr));
    return reinterpret_cast<float*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, shape, 0)[0x30 / 4])(shape, nullptr);
}

static void Movement_SetCollisionEnabled(void* object, int enabled)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, object, 0)[0x6C / 4])(object, nullptr, enabled);
}

static void* Movement_QuerySegment(C_0082dfd0* self, float* segment)
{
    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(loader, nullptr));
    void* queries = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x3C / 4])(controller, nullptr);
    return reinterpret_cast<void*(__fastcall*)(void*, void*, int, float*, uint32_t*, int)>(
        CSF_AT(void**, queries, 0)[0x34 / 4])(
            queries, nullptr, 0, segment, &self->field_0030, 3);
}

static void Movement_ReadHit(void* query, uint32_t* hit)
{
    // The query writes a full hit object, not merely the position and normal.
    std::memset(hit, 0, 0x94);
    CSF_AT(float, hit, 0x00) = FLT_MAX;
    CSF_AT(int, hit, 0x1C) = -1;
    CSF_AT(uint8_t, hit, 0x20) = 0xFF;
    CSF_AT(int, hit, 0x2C) = -1;
    CSF_AT(uint8_t, hit, 0x30) = 0xFF;
    CSF_AT(int, hit, 0x64) = 10;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, query, 0)[0x0C / 4])(query, nullptr, hit);
}

static char Movement_TestMotion(C_0082dfd0* self, char alternate,
                               float* direction, float* result)
{
    if (alternate)
        return sub_405370(reinterpret_cast<void*>(self->field_0008),
                         nullptr, direction, result);
    return sub_4053A0(reinterpret_cast<void*>(self->field_0008),
                     nullptr, direction, result,
                     reinterpret_cast<int*>(&self->field_0030));
}

static float Movement_PointerAsFloat(void* pointer)
{
    union {
        uint32_t bits;
        float value;
    } conversion;
    conversion.bits = reinterpret_cast<uint32_t>(pointer);
    return conversion.value;
}

static bool Movement_HasExternalVelocity(C_0082dfd0* self)
{
    return !(self->field_0090 == g_ZeroVectorX &&
             self->field_0094 == g_ZeroVectorY &&
             self->field_0098 == g_ZeroVectorZ);
}

static char __fastcall C_0082dfd0_MoveWithCollisions(
    C_0082dfd0* self, void* /*edx*/, float* displacement, float elapsed)
{
    // The two leading words and the following 0x5C-byte collision object
    // are one contiguous output area for the movement solver.
    alignas(4) float motion[0x64 / 4] = {};
    CSF_AT(float, motion, 0x2C) = 1.0;
    CSF_AT(uint8_t, motion, 0x48) = 1;

    float direction[3] = {};
    float dimensions[3];
    float originalPosition[0x1C / 4] = {};
    float requestedUnit[3] = {};
    float scratch[0x30 / 4] = {};
    float unitScratch[3] = {};
    float currentPosition[3];
    float actualDisplacement[4] = {};
    float actualUnit[4] = {};
    float retryPosition[4] = {};
    float segment[0x28 / 4] = {};
    uint32_t hit[0x94 / 4] = {};
    uint32_t request[0x28 / 4] = {};

    int iterations = 0;
    int interactionAttempts = 0;
    unsigned int stepAttempts = 0;
    int savedCollisionObject = 0;

    void* owner = Movement_Object(self);
    bool noGravity = false;
    if (CSF_AT(uint32_t, owner, 0x50) == 0)
        noGravity = (CSF_AT(uint32_t, owner, 0x2E0) & 0x10000) != 0;

    char multipleSupports = CSF_AT(char,
        reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0);
    uint8_t lateralConstraint = self->field_008c;
    self->field_008b = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x04) =
        lateralConstraint;

    char alternateSolver = 0;
    if (CSF_AT(uint32_t, Movement_Object(self), 0x50) == 0)
        alternateSolver = static_cast<char>(
            IsField1BCNonNegative(Movement_Object(self), nullptr));

    if (multipleSupports) {
        unsigned int index = 0;
        unsigned int offset = 0;
        if (self->field_0154) {
            do {
                void* record = reinterpret_cast<void*>(self->field_014c + offset);
                if (CSF_AT(int, record, 0x08) >= 0) {
                    int object = CHandleTable_Resolve(
                        g_pCurrentScene, nullptr, reinterpret_cast<int>(record));
                    if (object)
                        Movement_SetCollisionEnabled(reinterpret_cast<void*>(object), 0);
                }
                ++index;
                offset += 12;
            } while (index < self->field_0154);
        }
        savedCollisionObject = 0;
    } else if (static_cast<int32_t>(self->field_0148) >= 0) {
        savedCollisionObject = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(&self->field_0140));
        if (savedCollisionObject) {
            Movement_SetCollisionEnabled(
                reinterpret_cast<void*>(savedCollisionObject), 0);
        } else {
            CSF_AT(uint32_t, self, 0x144) = UINT32_MAX;
            self->field_0148 = UINT32_MAX;
        }
    }

    Movement_CopyVector(dimensions, Movement_GetDimensions(self));
    Movement_CopyVector(originalPosition, Movement_Position(Movement_Object(self)));
    direction[0] = displacement[0];
    direction[1] = 0.0;
    direction[2] = displacement[2];

    if (direction[2] * direction[2] + direction[0] * direction[0] > 0.0) {
        Movement_UnitDirection(direction, requestedUnit);
        if (self->field_008c)
            direction[0] += 0.1;
        if (self->field_008a)
            direction[1] = displacement[1];

        while (Movement_LengthSquared(direction) > 0.0) {
            if (!iterations) {
                float* center = sub_411EF0(self, nullptr, scratch);
                currentPosition[0] = center[0];
                currentPosition[1] = dimensions[1] + center[1];
                currentPosition[2] = center[2];

                float radius = dimensions[0] >= dimensions[2]
                    ? dimensions[0] : dimensions[2];
                float* unit = Movement_UnitDirection(direction, unitScratch);
                Movement_CopyVector(segment, currentPosition);
                segment[3] = radius * unit[0] * 0.60000002 + currentPosition[0];
                segment[4] = radius * unit[1] * 0.60000002 + currentPosition[1];
                segment[5] = radius * unit[2] * 0.60000002 + currentPosition[2];

                void* query = Movement_QuerySegment(self, segment);
                if (query) {
                    Movement_ReadHit(query, hit);
                    float normal[4] = {
                        CSF_AT(float, hit, 0x10), 0.0,
                        CSF_AT(float, hit, 0x18), 0.0
                    };
                    NormalizeVector3(normal, nullptr);

                    alignas(4) uint8_t plane[0x2C] = {};
                    CSF_AT(float, plane, 0x00) = CSF_AT(float, hit, 0x04);
                    CSF_AT(float, plane, 0x04) = CSF_AT(float, hit, 0x08);
                    CSF_AT(float, plane, 0x08) = CSF_AT(float, hit, 0x0C);
                    CSF_AT(float, plane, 0x0C) = normal[0];
                    CSF_AT(float, plane, 0x10) = normal[1];
                    CSF_AT(float, plane, 0x14) = normal[2];
                    CSF_AT(uint8_t, plane, 0x28) = 1;

                    float point[0x24 / 4] = {};
                    point[0] = CSF_AT(float, hit, 0x04) + direction[0];
                    point[1] = CSF_AT(float, hit, 0x08) + direction[1];
                    point[2] = CSF_AT(float, hit, 0x0C) + direction[2];
                    float projected[3] = {};
                    S_00526d40_ProjectPointToPlane(
                        reinterpret_cast<S_00526d40*>(plane), nullptr,
                        projected, point);
                    direction[0] = projected[0] - CSF_AT(float, hit, 0x04);
                    direction[1] = projected[1] - CSF_AT(float, hit, 0x08);
                    direction[2] = projected[2] - CSF_AT(float, hit, 0x0C);
                }
            }

            char collided = Movement_TestMotion(
                self, alternateSolver, direction, motion);
            if (collided) {
                void* obstacle = CSF_AT(void*, motion, 0x4C);
                if (obstacle) {
                    if (CSF_AT(uint32_t, obstacle, 0x170) & 0x2000) {
                        void* handled;
                        if (multipleSupports) {
                            handled = sub_413C60(
                                self, nullptr, reinterpret_cast<int>(obstacle),
                                CSF_AT(float, motion, 0x55));
                        } else {
                            handled = reinterpret_cast<void*>(sub_412400(
                                self, nullptr, Movement_PointerAsFloat(obstacle),
                                savedCollisionObject, 0));
                        }
                        if (handled) {
                            if (!multipleSupports)
                                savedCollisionObject = reinterpret_cast<int>(handled);
                            if (++interactionAttempts > 10) {
                                Movement_SetPosition(self, originalPosition);
                                Movement_RefreshShape(self);
                                break;
                            }
                            continue;
                        }
                        obstacle = CSF_AT(void*, motion, 0x4C);
                    }

                    if (obstacle && !iterations) {
                        if (CSF_AT(int, obstacle, 0x50) == 12) {
                            void* component =
                                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                                    CSF_AT(void**, obstacle, 0)[0x28 / 4])(
                                        obstacle, nullptr);
                            if (component) {
                                void* physics =
                                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                                        CSF_AT(void**, component, 0)[0x78 / 4])(
                                            component, nullptr);
                                if (physics) {
                                    void* bounds =
                                        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                                            CSF_AT(void**, component, 0)[0x70 / 4])(
                                                component, nullptr);
                                    float* size =
                                        reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                                            CSF_AT(void**, bounds, 0)[0x30 / 4])(
                                                bounds, nullptr);
                                    float objectSize[3];
                                    Movement_CopyVector(objectSize, size);
                                    void* transform =
                                        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                                            CSF_AT(void**, component, 0)[0x18 / 4])(
                                                component, nullptr);
                                    float halfHeight = objectSize[1] * 0.5;
                                    float* position =
                                        reinterpret_cast<float*(__fastcall*)(void*, void*)>(
                                            CSF_AT(void**, transform, 0)[0x10 / 4])(
                                                transform, nullptr);
                                    float applicationPoint[3] = {
                                        position[0], halfHeight + position[1],
                                        position[2]
                                    };
                                    float scale = CSF_AT(float,
                                        reinterpret_cast<void*>(GetGlobalValue_8BC1E0()),
                                        0x95C);
                                    float impulse[3] = {
                                        direction[0] * scale,
                                        direction[1] * scale,
                                        direction[2] * scale
                                    };
                                    reinterpret_cast<void(__fastcall*)(
                                        void*, void*, float*, float*, int)>(
                                            CSF_AT(void**, physics, 0)[0x44 / 4])(
                                                physics, nullptr, impulse,
                                                applicationPoint, 0);
                                }
                            }
                            obstacle = CSF_AT(void*, motion, 0x4C);
                        }
                        if (CSF_AT(uint32_t, obstacle, 0x170) & 0x2000) {
                            int context = CSF_AT(int,
                                reinterpret_cast<void*>(GetGlobalValue_8BC1E0()),
                                0xA78);
                            void* object = Movement_Object(self);
                            uint32_t* objectHandle = reinterpret_cast<uint32_t*>(
                                reinterpret_cast<uint8_t*>(object) + 0x1C);
                            int position = GetObjectPositionVector(object, nullptr);
                            sub_4460A0(request, nullptr, 2,
                                reinterpret_cast<int>(objectHandle),
                                reinterpret_cast<uint32_t*>(position), context);
                            SceneRequest_Submit(
                                CSF_AT(void*, g_pCurrentScene, 0xB0), nullptr,
                                reinterpret_cast<int>(request));
                            request[2] = reinterpret_cast<uint32_t>(
                                CSF_VTBL(0x0082D410));
                        }
                    }
                }

                bool canStep = stepAttempts < 5 && self->field_0088;
                float heightDifference = 0.0;
                if (canStep) {
                    void* object = Movement_Object(self);
                    canStep = !(CSF_AT(uint32_t, object, 0x50) == 0 &&
                                (CSF_AT(uint32_t, object, 0x2E0) & 0x10000));
                    if (canStep)
                        canStep = CSF_AT(int, object, 0x1C4) == 1;
                    if (canStep) {
                        float* position = Movement_Position(object);
                        heightDifference =
                            CSF_AT(float, motion, 0x5C) - position[1];
                        float fraction = heightDifference / dimensions[1];
                        canStep = !(fraction < 0.1 || fraction > 0.30000001);
                    }
                }
                if (canStep) {
                    float increment = heightDifference * 0.25;
                    float* position = Movement_Position(Movement_Object(self));
                    retryPosition[0] = position[0];
                    retryPosition[1] = increment + position[1];
                    retryPosition[2] = position[2];
                    Movement_SetPosition(self, retryPosition);
                    Movement_RefreshShape(self);
                    direction[0] = displacement[0];
                    direction[1] = 0.0;
                    direction[2] = displacement[2];
                    iterations = 0;
                    ++stepAttempts;
                    continue;
                }

                void* object = Movement_Object(self);
                if (CSF_AT(int, object, 0x1C4) != 1) {
                    float* position = Movement_Position(object);
                    if (CSF_AT(float, motion, 0x18) > 0.0 &&
                        (CSF_AT(float, motion, 0x5C) - position[1]) /
                            dimensions[1] >= 0.30000001)
                        CSF_AT(float, motion, 0x18) = 0.0;
                }
            }

            Movement_SetPosition(self, reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(motion) + 0x08));
            Movement_RefreshShape(self);
            Movement_CopyVector(direction, reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(motion) + 0x14));
            uint8_t constrained = self->field_008c;
            if (constrained) {
                direction[0] = 0.0;
                direction[2] = 0.0;
            }
            if (++iterations <= (constrained ? 1 : 10))
                continue;

            if (CSF_AT(uint32_t, motion, 0x00) == 1) {
                retryPosition[0] = CSF_AT(float, motion, 0x08);
                retryPosition[1] = CSF_AT(float, motion, 0x0C) + 0.1;
                retryPosition[2] = CSF_AT(float, motion, 0x10);
                Movement_SetPosition(self, retryPosition);
            } else if (Movement_LengthSquared(direction) <= 0.0) {
                Movement_SetPosition(self, originalPosition);
            } else {
                float* unit = Movement_UnitDirection(direction, unitScratch);
                retryPosition[0] = originalPosition[0] - unit[0];
                retryPosition[1] = originalPosition[1] - unit[1];
                retryPosition[2] = originalPosition[2] - unit[2];
                Movement_SetPosition(self, retryPosition);
            }
            Movement_RefreshShape(self);
            break;
        }

        float* position = Movement_Position(Movement_Object(self));
        actualDisplacement[0] = position[0] - originalPosition[0];
        actualDisplacement[1] = position[1] - originalPosition[1];
        actualDisplacement[2] = position[2] - originalPosition[2];
        if (Movement_LengthSquared(actualDisplacement) >= 0.1) {
            Movement_UnitDirection(actualDisplacement, retryPosition);
            if (!Movement_HasExternalVelocity(self) &&
                requestedUnit[0] * retryPosition[0] +
                requestedUnit[1] * retryPosition[1] +
                requestedUnit[2] * retryPosition[2] < 0.0) {
                Movement_SetPosition(self, originalPosition);
                Movement_RefreshShape(self);
            }
        }
    }

    sub_411EF0(self, nullptr, requestedUnit);
    if (self->field_008a && self->field_00d4 > 0.0) {
        currentPosition[0] = requestedUnit[0];
        currentPosition[1] = requestedUnit[1] + dimensions[1] * 0.15000001;
        currentPosition[2] = requestedUnit[2];
        float launchDirection[4] = {
            CSF_AT(float, self, 0xB4),
            CSF_AT(float, self, 0xB8),
            CSF_AT(float, self, 0xBC), 0.0
        };
        float radius = dimensions[0] >= dimensions[2]
            ? dimensions[0] : dimensions[2];
        Movement_CopyVector(segment, currentPosition);
        requestedUnit[1] = radius * (launchDirection[1] * -1.0);
        requestedUnit[2] = radius * launchDirection[2];
        launchDirection[0] =
            launchDirection[0] * radius * 0.69999999 + currentPosition[0];
        launchDirection[1] =
            requestedUnit[1] * 0.69999999 + currentPosition[1];
        launchDirection[2] =
            requestedUnit[2] * 0.69999999 + currentPosition[2];
        Movement_CopyVector(segment + 3, launchDirection);
        if (Movement_QuerySegment(self, segment))
            self->field_00d4 = 0.0;
    }

    Movement_CopyVector(originalPosition, Movement_Position(Movement_Object(self)));
    direction[0] = 0.0;
    direction[1] = displacement[1];
    direction[2] = 0.0;
    iterations = 0;
    interactionAttempts = 0;
    if (direction[1] * direction[1] > 0.0) {
        Movement_UnitDirection(direction, retryPosition);
        while (Movement_LengthSquared(direction) > 0.0) {
            if (self->field_008a)
                break;
            char collided = Movement_TestMotion(
                self, alternateSolver, direction, motion);
            void* obstacle = CSF_AT(void*, motion, 0x4C);
            if (collided && obstacle &&
                (CSF_AT(uint32_t, obstacle, 0x170) & 0x2000)) {
                if (multipleSupports) {
                    bool handled = sub_413C60(
                        self, nullptr, reinterpret_cast<int>(obstacle),
                        CSF_AT(float, motion, 0x55)) != nullptr;
                    if (!handled && CSF_AT(float, motion, 0x30) > 0.5) {
                        void* support = CSF_AT(void*, motion, 0x4C);
                        Movement_SetCollisionEnabled(support, 0);
                        if (self->field_0154 >= self->field_0150) {
                            (void)CSF_VTBL(0x00413570);
                            MissionScript_GrowEventRecords(
                                &self->field_014c, nullptr,
                                CSF_AT(int, self, 0x158));
                        }
                        uint32_t index = self->field_0154;
                        uint32_t records = self->field_014c;
                        self->field_0154 = index + 1;
                        void* record = reinterpret_cast<void*>(records + 12 * index);
                        CSF_AT(uint32_t, record, 0x04) =
                            CSF_AT(uint32_t, support, 0x20);
                        CSF_AT(uint32_t, record, 0x08) =
                            CSF_AT(uint32_t, support, 0x24);
                        handled = true;
                    }
                    if (handled) {
                        if (++interactionAttempts > 10) {
                            Movement_SetPosition(self, originalPosition);
                            Movement_RefreshShape(self);
                            break;
                        }
                        continue;
                    }
                } else if (CSF_AT(float, motion, 0x30) > 0.5) {
                    float angles[2] = {};
                    void* object = Movement_Object(self);
                    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                        CSF_AT(void**, object, 0)[0x38 / 4])(
                            object, nullptr, angles);
                    GfxMatrix_EulerAnglesToDirection(
                        angles, nullptr, currentPosition);
                    currentPosition[1] = 0.0;
                    if (Movement_SquareRoot(
                            currentPosition[0] * currentPosition[0] +
                            currentPosition[2] * currentPosition[2]) == 0.0) {
                        currentPosition[0] = 1.0;
                        currentPosition[1] = 0.0;
                        currentPosition[2] = 0.0;
                    } else {
                        NormalizeVector3(currentPosition, nullptr);
                    }
                    requestedUnit[0] = currentPosition[0] * 750.0;
                    requestedUnit[1] = currentPosition[1] * 750.0;
                    requestedUnit[2] = currentPosition[2] * 750.0;
                    reinterpret_cast<void(__fastcall*)(
                        C_0082dfd0*, void*, float*, float*, int)>(
                            CSF_AT(void**, self, 0)[0x54 / 4])(
                                self, nullptr, requestedUnit, &g_ZeroVectorX, 0);
                }
            }

            Movement_SetPosition(self, reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(motion) + 0x08));
            Movement_RefreshShape(self);
            Movement_CopyVector(direction, reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(motion) + 0x14));
            if (++iterations > 10) {
                if (CSF_AT(uint32_t, motion, 0x00) == 1)
                    Movement_SetPosition(self, reinterpret_cast<float*>(
                        reinterpret_cast<uint8_t*>(motion) + 0x08));
                else
                    Movement_SetPosition(self, originalPosition);
                Movement_RefreshShape(self);
                break;
            }
        }

        float* position = Movement_Position(Movement_Object(self));
        actualDisplacement[0] = position[0] - originalPosition[0];
        actualDisplacement[1] = position[1] - originalPosition[1];
        actualDisplacement[2] = position[2] - originalPosition[2];
        if (Movement_LengthSquared(actualDisplacement) > 0.0) {
            Movement_UnitDirection(actualDisplacement, requestedUnit);
            if (!Movement_HasExternalVelocity(self) &&
                retryPosition[0] * requestedUnit[0] +
                retryPosition[1] * requestedUnit[1] +
                retryPosition[2] * requestedUnit[2] < 0.0) {
                Movement_SetPosition(self, originalPosition);
                Movement_RefreshShape(self);
            }
        }
    }

    if (!self->field_0088) {
        sub_411EF0(self, nullptr, requestedUnit);
        float probeOrigin[3];
        probeOrigin[0] = requestedUnit[0];
        probeOrigin[1] = requestedUnit[1] + dimensions[1] * 0.75;
        probeOrigin[2] = requestedUnit[2];
        currentPosition[0] = requestedUnit[0];
        currentPosition[1] = dimensions[1] * 0.34999999 + probeOrigin[1];
        currentPosition[2] = requestedUnit[2];
        Movement_CopyVector(segment, probeOrigin);
        Movement_CopyVector(segment + 3, currentPosition);
        void* query = Movement_QuerySegment(self, segment);
        if (query) {
            self->field_008b = 1;
            Movement_ReadHit(query, hit);
            actualUnit[0] = CSF_AT(float, hit, 0x04) - probeOrigin[0];
            actualUnit[1] = CSF_AT(float, hit, 0x08) - probeOrigin[1];
            actualUnit[2] = CSF_AT(float, hit, 0x0C) - probeOrigin[2];
            dimensions[0] = currentPosition[0] - probeOrigin[0];
            dimensions[1] = currentPosition[1] - probeOrigin[1];
            dimensions[2] = currentPosition[2] - probeOrigin[2];
            Movement_CopyVector(actualDisplacement,
                                Movement_Position(Movement_Object(self)));
            actualDisplacement[1] -=
                Movement_SquareRoot(Movement_LengthSquared(dimensions)) -
                Movement_SquareRoot(Movement_LengthSquared(actualUnit));
            Movement_SetPosition(self, actualDisplacement);
            Movement_RefreshShape(self);
        }
    }

    if (!noGravity && !self->field_0089) {
        float lengthSquared = Movement_LengthSquared(displacement);
        float* gravityDirection;
        if (lengthSquared == 0.0 || lengthSquared != lengthSquared) {
            requestedUnit[0] = -g_AxisY;
            requestedUnit[1] = -g_AxisY_Y;
            requestedUnit[2] = -g_AxisY_Z;
            gravityDirection = requestedUnit;
        } else {
            gravityDirection = Movement_UnitDirection(displacement, unitScratch);
        }
        sub_412EE0(self, nullptr, elapsed,
                  reinterpret_cast<int>(gravityDirection));
    }

    // Re-enable temporarily excluded supports and discard stale handles.
    // The packet omits the floating-point expressions preceding the support
    // distance flags. The horizontal extent test below is an inferred test.
    if (multipleSupports) {
        unsigned int index = 0;
        unsigned int offset = 0;
        if (self->field_0154) {
            do {
                void* record = reinterpret_cast<void*>(self->field_014c + offset);
                if (CSF_AT(int, record, 0x08) >= 0) {
                    int resolved = CHandleTable_Resolve(
                        g_pCurrentScene, nullptr, reinterpret_cast<int>(record));
                    bool remove = !resolved;
                    if (resolved) {
                        void* support = reinterpret_cast<void*>(resolved);
                        Movement_SetCollisionEnabled(support, 1);
                        Movement_CopyVector(actualUnit, Movement_GetDimensions(self));
                        float radius = actualUnit[0] >= actualUnit[2]
                            ? actualUnit[0] : actualUnit[2];
                        Movement_CopyVector(actualDisplacement,
                                            Movement_Position(support));
                        float* position = Movement_Position(Movement_Object(self));
                        float x = position[0] - actualDisplacement[0];
                        float z = position[2] - actualDisplacement[2];
                        float distanceSquared = x * x + z * z;
                        remove = !(distanceSquared <= radius * radius);
                    }
                    if (remove) {
                        uint32_t last = self->field_0154 - 1;
                        self->field_0154 = last;
                        if (index != last) {
                            _memcpy(
                                reinterpret_cast<void*>(self->field_014c + offset),
                                reinterpret_cast<void*>(self->field_014c + offset + 12),
                                12 * (last - index));
                        }
                    } else {
                        ++index;
                        offset += 12;
                    }
                }
            } while (index < self->field_0154);
        }
    } else if (static_cast<int32_t>(self->field_0148) >= 0) {
        int resolved = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(&self->field_0140));
        bool remove = !resolved;
        if (resolved) {
            void* support = reinterpret_cast<void*>(resolved);
            Movement_SetCollisionEnabled(support, 1);
            Movement_CopyVector(actualUnit, Movement_GetDimensions(self));
            Movement_CopyVector(actualDisplacement, Movement_Position(support));
            float* position = Movement_Position(Movement_Object(self));
            float x = position[0] - actualDisplacement[0];
            float z = position[2] - actualDisplacement[2];
            float radius = actualUnit[0] >= actualUnit[2]
                ? actualUnit[0] : actualUnit[2];
            remove = !(x * x + z * z <= radius * radius);
        }
        if (remove) {
            CSF_AT(uint32_t, self, 0x144) = UINT32_MAX;
            self->field_0148 = UINT32_MAX;
        }
    }

    uint8_t supported = self->field_0089 | self->field_0088;
    char result = static_cast<char>(self->field_008b);
    self->field_0088 = supported;
    if (result)
        self->field_0084 = self->field_013c * -980.0 * elapsed;
    uint8_t contacted = static_cast<uint8_t>(result) | self->field_008e;
    self->field_008c &= supported == 0;
    self->field_008e = contacted;
    return result;
}

HOOK(0x00413DE0, C_0082dfd0_MoveWithCollisions, "auto");
