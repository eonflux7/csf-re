#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResetRotationEntry, 0x004068B0, (void* self, void* edx));
CSF_FUNC(double, __cdecl, GetAngularDifference, 0x004068E0, (float a1, float a2));
CSF_FUNC(uint32_t*, __fastcall, TransformRotationVector, 0x004069F0, (void* self, void* edx, uint32_t* a2, int* a3));
CSF_FUNC(void, __fastcall, UpdateRotationAxis, 0x00407080, (void* self, void* edx, float a2, int a3, float a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_SetYRotation, 0x006AE2A0, (S_006ada10* self, void* , float angle));

CSF_GLOBAL(float, g_RotationRadiansToDegrees, 0x0082D750);
CSF_GLOBAL(float, g_RotationDegreesToRadians, 0x0082D754);
CSF_GLOBAL(float, g_RotationTolerance, 0x0082D758);
CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static float EncodeAxisAddress(float* axis)
{
    const std::uint32_t address = reinterpret_cast<std::uint32_t>(axis);
    float encoded;
    std::memcpy(&encoded, &address, sizeof(encoded));
    return encoded;
}

static void __fastcall C_0082d77c_UpdateEntryRotation(
    C_0082d77c* self, void* /*edx*/, float elapsed, int entryIndex)
{
    auto* entry = reinterpret_cast<std::uint8_t*>(
        self->field_000c + 44 * static_cast<std::uint32_t>(entryIndex));
    const int objectIndex = CSF_AT(int, entry, 0x8);
    if (objectIndex == -1)
        return;

    UpdateRotationAxis(self, nullptr, elapsed, entryIndex, EncodeAxisAddress(&g_AxisY));
    UpdateRotationAxis(self, nullptr, elapsed, entryIndex, EncodeAxisAddress(&g_AxisX));

    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* scene = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* controller = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, scene, 0)[0x68 / 4])(scene, nullptr);
    if (!controller)
        return;

    void* firstObject = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, controller, 0)[0x6C / 4])(controller, nullptr, objectIndex);
    void* secondObject = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, controller, 0)[0x70 / 4])(controller, nullptr, objectIndex);

    if (firstObject && secondObject)
    {
        owner = reinterpret_cast<void*>(self->field_0008);
        scene = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
        if (CSF_AT(int, scene, 0x78) != -1082130432)
        {
            const float pitch = static_cast<float>(GetAngularDifference(
                CSF_AT(float, entry, 0x10), CSF_AT(float, entry, 0xC)));
            const float yaw = static_cast<float>(GetAngularDifference(
                CSF_AT(float, entry, 0x20), CSF_AT(float, entry, 0x1C)));

            std::uint32_t yawAxis[3]{};
            reinterpret_cast<void(__fastcall*)(void*, void*, void*, float*)>(
                CSF_AT(void**, secondObject, 0)[0x30 / 4])(
                    secondObject, nullptr, yawAxis, &g_AxisY);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, void*, float)>(
                CSF_AT(void**, controller, 0)[0x78 / 4])(
                    controller, nullptr, objectIndex, yawAxis,
                    yaw * g_RotationRadiansToDegrees);

            owner = reinterpret_cast<void*>(self->field_0008);
            scene = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
            void* transform = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, scene, 0)[0x18 / 4])(scene, nullptr);

            std::uint32_t borrowedMatrix[2]{};
            auto* matrixResult =
                reinterpret_cast<std::uint32_t**(__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, transform, 0)[0x8C / 4])(
                        transform, nullptr, borrowedMatrix);
            std::uint32_t* sourceVector = *matrixResult;
            std::uint32_t rotatedVector[3];
            rotatedVector[0] = sourceVector[0];
            rotatedVector[1] = sourceVector[1];
            rotatedVector[2] = sourceVector[2];

            if (CSF_AT(std::uint8_t, borrowedMatrix, 0x4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, borrowedMatrix, 0));

            std::uint32_t rotationMatrix[4]{};
            CSF_AT(std::uint32_t*, rotationMatrix, 0) = RwMatrixCreate();
            CSF_AT(std::uint8_t, rotationMatrix, 0x4) = 1;
            GfxMatrix_SetYRotation(
                reinterpret_cast<S_006ada10*>(rotationMatrix), nullptr,
                yaw * g_RotationDegreesToRadians);

            std::uint32_t vectorResult = 0;
            sourceVector = TransformRotationVector(
                rotatedVector, nullptr, &vectorResult,
                reinterpret_cast<int*>(rotationMatrix));
            rotatedVector[0] = sourceVector[0];
            rotatedVector[1] = sourceVector[1];
            rotatedVector[2] = sourceVector[2];

            std::uint32_t pitchAxis[3]{};
            reinterpret_cast<void(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, secondObject, 0)[0x30 / 4])(
                    secondObject, nullptr, pitchAxis, rotatedVector);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, void*, float)>(
                CSF_AT(void**, controller, 0)[0x7C / 4])(
                    controller, nullptr, objectIndex, pitchAxis,
                    pitch * g_RotationRadiansToDegrees);

            if (CSF_AT(std::uint8_t, rotationMatrix, 0x4))
                RwMatrixDestroy(CSF_AT(std::uint32_t*, rotationMatrix, 0));
        }
    }

    if (!CSF_AT(std::uint32_t, entry, 0x4)
        && std::fabs(GetAngularDifference(
            CSF_AT(float, entry, 0x10), CSF_AT(float, entry, 0xC)))
            < g_RotationTolerance
        && std::fabs(GetAngularDifference(
            CSF_AT(float, entry, 0x20), CSF_AT(float, entry, 0x1C)))
            < g_RotationTolerance)
    {
        ResetRotationEntry(entry, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, float*, float)>(
            CSF_AT(void**, controller, 0)[0x78 / 4])(
                controller, nullptr, objectIndex, &g_ZeroVectorX, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, float*)>(
            CSF_AT(void**, controller, 0)[0x80 / 4])(
                controller, nullptr, objectIndex, &g_ZeroVectorX);
    }
}

HOOK(0x00407490, C_0082d77c_UpdateEntryRotation, "auto");
