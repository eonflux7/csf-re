#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_GrowCorrectionArray, 0x00419920, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_VehicleCorrectionScale, 0x0082E070);
CSF_GLOBAL(float, g_InitialMinimumWheelHeight, 0x0082E1DC);

static void __fastcall C_0082e1e8_AppendWheelHeightCorrection(
    C_0082e1e8* self, void* /*edx*/, std::uint32_t* settings)
{
    const int wheelCount = static_cast<int>(self->field_003c);
    int firstLeft = static_cast<int>(settings[2]);
    if (firstLeft == -1)
        firstLeft = 0;

    int firstRight = static_cast<int>(settings[3]);
    if (firstRight == -1)
        firstRight = 0;

    int lastLeft = static_cast<int>(settings[4]);
    if (lastLeft == -1)
        lastLeft = wheelCount - 1;

    int lastRight = static_cast<int>(settings[5]);
    if (lastRight == -1)
        lastRight = wheelCount - 1;

    float minimumHeight = g_InitialMinimumWheelHeight;
    float maximumHeight = 0.0f;
    int highestWheel = -1;

    if (wheelCount > 0)
    {
        auto* rightWheels = reinterpret_cast<std::uint8_t*>(self->field_0044);
        auto* leftWheels = reinterpret_cast<std::uint8_t*>(self->field_0034);

        for (int index = 0; index < wheelCount; ++index)
        {
            auto* leftWheel = leftWheels + 32 * index;
            auto* rightWheel = rightWheels + 32 * index;

            if (CSF_AT(float, leftWheel, 0x10) > maximumHeight)
            {
                maximumHeight = CSF_AT(float, leftWheel, 0x10);
                highestWheel = index;
            }
            if (CSF_AT(float, rightWheel, 0x10) > maximumHeight)
            {
                maximumHeight = CSF_AT(float, rightWheel, 0x10);
                highestWheel = index;
            }
            if (minimumHeight > CSF_AT(float, leftWheel, 0x10))
                minimumHeight = CSF_AT(float, leftWheel, 0x10);
            if (minimumHeight > CSF_AT(float, rightWheel, 0x10))
                minimumHeight = CSF_AT(float, rightWheel, 0x10);
        }
    }

    if (minimumHeight > g_fZero)
    {
        auto* position = reinterpret_cast<void*>(
            GetObjectPositionVector(reinterpret_cast<void*>(self->field_0004), nullptr));

        int adjustedPosition[3];
        adjustedPosition[0] = CSF_AT(int, position, 0x0);
        float height = CSF_AT(float, position, 0x4);
        adjustedPosition[2] = CSF_AT(int, position, 0x8);
        height += minimumHeight;
        std::memcpy(&adjustedPosition[1], &height, sizeof(height));

        SceneLoader_ConfigureFocusEntry(
            reinterpret_cast<void*>(self->field_0004), nullptr, adjustedPosition);
        maximumHeight -= minimumHeight;
    }

    if (maximumHeight > g_fZero)
    {
        void* returnedObject = reinterpret_cast<void*>(
            C_0082dd78_DispatchReturnedObject(
                reinterpret_cast<C_0082dd78*>(self), nullptr));

        void* dimensions =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, returnedObject, 0)[0x30 / 4])(
                    returnedObject, nullptr);

        const float strength =
            maximumHeight / (CSF_AT(float, dimensions, 0x8) * g_HalfDurationScale)
            * g_VehicleCorrectionScale;

        std::uint32_t correction[7] = {};
        std::uint32_t matrixResult[2] = {0, static_cast<std::uint32_t>(firstRight)};
        std::uint32_t extendedMatrixResult[0x28 / sizeof(std::uint32_t)] = {};

        if (highestWheel >= wheelCount / 2)
        {
            void* entry = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                CSF_AT(int, reinterpret_cast<void*>(self->field_0034), 32 * firstLeft)));

            void* matrixReference =
                reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, entry, 0)[0x3C / 4])(
                        entry, nullptr, extendedMatrixResult);
            void* matrix = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, matrixReference, 0));

            correction[0] = CSF_AT(std::uint32_t, matrix, 0x30);
            correction[1] = CSF_AT(std::uint32_t, matrix, 0x34);
            correction[2] = CSF_AT(std::uint32_t, matrix, 0x38);
            if (static_cast<std::uint8_t>(extendedMatrixResult[1]))
                RwMatrixDestroy(
                    reinterpret_cast<std::uint32_t*>(extendedMatrixResult[0]));

            entry = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                CSF_AT(int, reinterpret_cast<void*>(self->field_0044),
                       32 * static_cast<int>(matrixResult[1]))));

            matrixReference =
                reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, entry, 0)[0x3C / 4])(
                        entry, nullptr, matrixResult);
            matrix = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, matrixReference, 0));

            correction[3] = CSF_AT(std::uint32_t, matrix, 0x30);
            correction[4] = CSF_AT(std::uint32_t, matrix, 0x34);
            correction[5] = CSF_AT(std::uint32_t, matrix, 0x38);
            if (static_cast<std::uint8_t>(matrixResult[1]))
                RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixResult[0]));

            std::memcpy(&correction[6], &strength, sizeof(strength));
        }
        else
        {
            void* entry = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                CSF_AT(int, reinterpret_cast<void*>(self->field_0034), 32 * lastLeft)));

            void* matrixReference =
                reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, entry, 0)[0x3C / 4])(
                        entry, nullptr, matrixResult);
            void* matrix = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, matrixReference, 0));

            correction[0] = CSF_AT(std::uint32_t, matrix, 0x30);
            correction[1] = CSF_AT(std::uint32_t, matrix, 0x34);
            correction[2] = CSF_AT(std::uint32_t, matrix, 0x38);
            if (static_cast<std::uint8_t>(matrixResult[1]))
                RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixResult[0]));

            entry = reinterpret_cast<void*>(GetTableEntryDwordByIndex(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                CSF_AT(int, reinterpret_cast<void*>(self->field_0044), 32 * lastRight)));

            matrixReference =
                reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, entry, 0)[0x3C / 4])(
                        entry, nullptr, matrixResult);
            matrix = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, matrixReference, 0));

            correction[3] = CSF_AT(std::uint32_t, matrix, 0x30);
            correction[4] = CSF_AT(std::uint32_t, matrix, 0x34);
            correction[5] = CSF_AT(std::uint32_t, matrix, 0x38);
            if (static_cast<std::uint8_t>(matrixResult[1]))
                RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixResult[0]));

            const float signedStrength = -strength;
            std::memcpy(&correction[6], &signedStrength, sizeof(signedStrength));
        }

        if (settings[21] >= settings[20])
            Vehicle_GrowCorrectionArray(
                settings + 19, nullptr, static_cast<int>(settings[22]));

        void* destination = reinterpret_cast<void*>(
            settings[19] + 28 * settings[21]++);
        std::memcpy(destination, correction, 0x1C);
    }
}

HOOK(0x00419B50, C_0082e1e8_AppendWheelHeightCorrection, "auto");
