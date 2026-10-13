#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DynArray20_InsertAt, 0x00425B90, (void* self, void* , int index, const uint32_t* item));
CSF_FUNC(int, __fastcall, CPlayerCharacter_DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, float angle));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxBBoxMath_Vec3ScaleByReciprocal, 0x006AB830, (void* self, void* edx, float a2));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_FacingAngleOffset, 0x0082E044);
CSF_GLOBAL(float, g_MovementAcceleration, 0x0082E074);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x0082FE84);

static int __fastcall Path_FinalizeForCharacter(
    void* self, void* /*edx*/, CPlayerCharacter* character)
{
    alignas(8) std::uint8_t initialDirectionBytes[0xC];
    alignas(8) std::uint8_t directionBytes[0x14];
    alignas(8) std::uint8_t forwardBytes[0x24];
    alignas(8) std::uint8_t matrixResultBytes[0x8];
    alignas(8) std::uint8_t firstItemBytes[0x14];
    alignas(8) std::uint8_t secondItemBytes[0x14];

    auto& initialDirection =
        *reinterpret_cast<S_00526d40*>(initialDirectionBytes);
    auto& direction = *reinterpret_cast<S_00526d40*>(directionBytes);

    void* points = static_cast<std::uint8_t*>(self) + 0x14;
    float* data = CSF_AT(float*, self, 0x14);

    CSF_AT(float, initialDirectionBytes, 0x0) = data[5] - data[0];
    CSF_AT(float, initialDirectionBytes, 0x4) = data[6] - data[1];
    CSF_AT(float, initialDirectionBytes, 0x8) = data[7] - data[2];

    float targetAngle = static_cast<float>(
        S_00526d40_GetVectorAngleRadians(&initialDirection, nullptr) *
        g_RadiansToDegrees);

    void* sceneObject = reinterpret_cast<void*>(reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr));
    void* transform =
        ((void* (__fastcall*)(void*, void*))
             CSF_AT(void**, sceneObject, 0)[0x18 / 4])(
            sceneObject, nullptr);

    float& facingAngle = CSF_AT(float, directionBytes, 0xC);
    facingAngle = static_cast<float>(
        ((double (__fastcall*)(void*, void*))
             CSF_AT(void**, transform, 0)[0x28 / 4])(
            transform, nullptr));

    if (std::fabs(targetAngle - facingAngle) < g_TransitionOne)
        return CPlayerCharacter_DispatchSceneObjectWithPayload(character, nullptr, targetAngle);

    float& forwardX = CSF_AT(float, forwardBytes, 0x0);
    float& forwardY = CSF_AT(float, forwardBytes, 0x4);
    float& forwardZ = CSF_AT(float, forwardBytes, 0x8);
    float& endX = CSF_AT(float, forwardBytes, 0xC);
    float& endY = CSF_AT(float, forwardBytes, 0x10);
    float& endZ = CSF_AT(float, forwardBytes, 0x14);
    float& startX = CSF_AT(float, forwardBytes, 0x18);
    float& startY = CSF_AT(float, forwardBytes, 0x1C);
    float& startZ = CSF_AT(float, forwardBytes, 0x20);

    data = CSF_AT(float*, points, 0);
    startX = data[0];
    startY = data[1];
    startZ = data[2];
    endX = data[5];
    endY = data[6];
    endZ = data[7];

    float& directionX = CSF_AT(float, directionBytes, 0x0);
    float& directionY = CSF_AT(float, directionBytes, 0x4);
    float& directionZ = CSF_AT(float, directionBytes, 0x8);
    directionX = endX - startX;
    directionY = endY - startY;
    directionZ = endZ - startZ;

    float distance = std::sqrt(
        static_cast<double>(directionZ) * directionZ +
        static_cast<double>(directionY) * directionY +
        static_cast<double>(directionX) * directionX);
    GfxBBoxMath_Vec3ScaleByReciprocal(&direction, nullptr, distance);

    while (distance < g_MovementAcceleration) {
        if (CSF_AT(std::uint32_t, self, 0x1C) - 1 <= 1)
            break;

        std::uint32_t remaining =
            CSF_AT(std::uint32_t, points, 0x8) - 1;
        if (remaining == 1) {
            CSF_AT(std::uint32_t, points, 0x8) = remaining;
        } else {
            auto* destination =
                CSF_AT(std::uint8_t*, points, 0) + 20;
            CSF_AT(std::uint32_t, points, 0x8) = remaining;
            _memcpy(destination, destination + 20,
                    static_cast<std::uint32_t>(4 * (5 * remaining - 5)));
        }

        data = CSF_AT(float*, points, 0);
        endX = data[5];
        endY = data[6];
        endZ = data[7];
        directionX = endX - startX;
        directionY = endY - startY;
        directionZ = endZ - startZ;
        distance = std::sqrt(
            static_cast<double>(directionZ) * directionZ +
            static_cast<double>(directionY) * directionY +
            static_cast<double>(directionX) * directionX);
        GfxBBoxMath_Vec3ScaleByReciprocal(&direction, nullptr, distance);
    }

    sceneObject = reinterpret_cast<void*>(reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr));
    transform =
        ((void* (__fastcall*)(void*, void*))
             CSF_AT(void**, sceneObject, 0)[0x18 / 4])(
            sceneObject, nullptr);
    void* matrixResult =
        ((void* (__fastcall*)(void*, void*, void*))
             CSF_AT(void**, transform, 0)[0x8C / 4])(
            transform, nullptr, matrixResultBytes);

    auto* matrix = CSF_AT(std::uint8_t*, matrixResult, 0);
    forwardX = CSF_AT(float, matrix, 0x20);
    forwardY = CSF_AT(float, matrix, 0x24);
    forwardZ = CSF_AT(float, matrix, 0x28);

    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    forwardY = 0.0f;
    NormalizeVector3(reinterpret_cast<C_00833b88*>(forwardBytes), nullptr);

    double newAngle =
        S_00526d40_GetVectorAngleRadians(&direction, nullptr) *
        g_RadiansToDegrees;
    targetAngle = static_cast<float>(newAngle);
    if (std::fabs(newAngle - facingAngle) < g_TransitionOne)
        return CPlayerCharacter_DispatchSceneObjectWithPayload(character, nullptr, targetAngle);

    distance *= g_HalfDurationScale;
    float step = distance <= g_MovementAcceleration
        ? distance : g_MovementAcceleration;

    float& firstX = CSF_AT(float, firstItemBytes, 0x0);
    float& firstY = CSF_AT(float, firstItemBytes, 0x4);
    float& firstZ = CSF_AT(float, firstItemBytes, 0x8);

    float angleDifference = std::fabs(targetAngle - facingAngle);
    if (!(angleDifference >= g_FacingAngleOffset)) {
        endX = forwardX * step + startX;
        firstX = endX;
        endY = step * forwardY + startY;
        firstY = endY;
        endZ = forwardZ * step + startZ;
        firstZ = endZ;
        CSF_AT(float, firstItemBytes, 0xC) = step;
        CSF_AT(float, firstItemBytes, 0x10) = -1.0f;
        return DynArray20_InsertAt(
            points, nullptr, 1,
            reinterpret_cast<const std::uint32_t*>(firstItemBytes));
    }

    firstX = startX - forwardX * step;
    firstY = startY - step * forwardY;
    firstZ = startZ - forwardZ * step;
    CSF_AT(float, firstItemBytes, 0xC) = step;
    CSF_AT(float, firstItemBytes, 0x10) = step;
    DynArray20_InsertAt(
        points, nullptr, 1,
        reinterpret_cast<const std::uint32_t*>(firstItemBytes));

    float perpendicularZ = -forwardX;
    startX = forwardZ;
    startY = 0.0f;
    if ((endX - firstX) * forwardZ +
        (endY - firstY) * 0.0f +
        (endZ - firstZ) * perpendicularZ > g_fZero) {
        endY = 0.0f;
        endX = -forwardZ;
        startX = endX;
        startY = 0.0f;
        endZ = -perpendicularZ;
        startZ = endZ;
        perpendicularZ = endZ;
    }

    CSF_AT(std::uint32_t, secondItemBytes, 0xC) = 0;
    endX = startX * step + firstX;
    CSF_AT(float, secondItemBytes, 0x0) = endX;
    endY = startY * step + firstY;
    CSF_AT(float, secondItemBytes, 0x4) = endY;
    endZ = perpendicularZ * step + firstZ;
    CSF_AT(float, secondItemBytes, 0x8) = endZ;
    CSF_AT(float, secondItemBytes, 0x10) = step;

    DynArray20_InsertAt(
        points, nullptr, 2,
        reinterpret_cast<const std::uint32_t*>(secondItemBytes));
    int result = DynArray20_InsertAt(
        points, nullptr, 3,
        reinterpret_cast<const std::uint32_t*>(firstItemBytes));
    CSF_AT(std::uint8_t, self, 0x28) = 1;
    return result;
}

HOOK(0x00425EA0, Path_FinalizeForCharacter, "auto");
