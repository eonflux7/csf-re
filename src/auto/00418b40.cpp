#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_FindTableEntry, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(char, __stdcall, Vehicle_InitializeWheelData, 0x0040F6F0, (int a2, int a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(float, g_WheelTravelMultiplier, 0x0082E24C);
CSF_GLOBAL(float, g_NegativeWheelScale, 0x0082E250);

static char __fastcall C_0082e1e8_InitializeWheelOffsets(
    C_0082e1e8* self, void* /*edx*/, int data, int options)
{
    const char result = Vehicle_InitializeWheelData(data, options);

    self->field_0064 = 0.0f;
    self->field_0068 = 0.0f;
    self->field_006c = 0.0f;
    self->field_0070 = 0.0f;
    self->field_0074 = 0.0f;
    self->field_0078 = 0.0f;
    self->field_0088 = 0;
    self->field_0085 = 1;
    self->field_0086 = 1;
    self->field_007c = 0;
    self->field_007c = 1;

    self->field_0080 = CSF_AT(
        float, CSF_AT(void*, reinterpret_cast<void*>(data), 0x2C), 0x1D8);

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* vehicle = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    void* returnedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));
    void* dimensions = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, returnedObject, 0)[0x30 / 4])(returnedObject, nullptr);
    const float height = CSF_AT(float, dimensions, 0x4);

    void* matrixSource = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, vehicle, 0)[0x18 / 4])(vehicle, nullptr);
    void* positionSource = ((void* (__fastcall*)(void*, void*))
        CSF_AT(void**, vehicle, 0)[0x18 / 4])(vehicle, nullptr);

    alignas(4) std::uint8_t initialMatrixStorage[0x28]{};
    void* matrixView = ((void* (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, matrixSource, 0)[0x8C / 4])(
            matrixSource, nullptr, initialMatrixStorage);
    void* matrix = CSF_AT(void*, matrixView, 0);
    const float heightOffset = height * g_SoundActivityThreshold;
    const float offsetX = heightOffset * CSF_AT(float, matrix, 0x10);
    const float offsetY = heightOffset * CSF_AT(float, matrix, 0x14);
    const float offsetZ = heightOffset * CSF_AT(float, matrix, 0x18);

    float* position = ((float* (__fastcall*)(void*, void*))
        CSF_AT(void**, positionSource, 0)[0x10 / 4])(positionSource, nullptr);
    const float centerX = offsetX + position[0];
    const float centerY = offsetY + position[1];
    const float centerZ = offsetZ + position[2];

    if (CSF_AT(std::uint8_t, initialMatrixStorage, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, initialMatrixStorage, 0));

    if (self->field_003c != 0)
    {
        std::uint32_t index = 0;
        std::uint32_t byteOffset = 0;
        do
        {
            auto* leftWheel = reinterpret_cast<std::uint8_t*>(self->field_0034) + byteOffset;
            auto* rightWheel = reinterpret_cast<std::uint8_t*>(self->field_0044) + byteOffset;
            void* wheelObject = reinterpret_cast<void*>(Vehicle_FindTableEntry(
                reinterpret_cast<void*>(self->field_0030), nullptr,
                CSF_AT(int, leftWheel, 0)));

            if (wheelObject)
            {
                alignas(4) std::uint8_t wheelMatrixStorage[0x8]{};
                void* wheelView = ((void* (__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, wheelObject, 0)[0x3C / 4])(
                        wheelObject, nullptr, wheelMatrixStorage);
                void* wheelMatrix = CSF_AT(void*, wheelView, 0);
                const float wheelX = CSF_AT(float, wheelMatrix, 0x30);
                const float wheelY = CSF_AT(float, wheelMatrix, 0x34);
                const float wheelZ = CSF_AT(float, wheelMatrix, 0x38);
                if (CSF_AT(std::uint8_t, wheelMatrixStorage, 0x4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, wheelMatrixStorage, 0));

                void* axisSource = ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, vehicle, 0)[0x18 / 4])(vehicle, nullptr);
                alignas(4) std::uint8_t firstAxisStorage[0x8]{};
                void* firstAxisView = ((void* (__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, axisSource, 0)[0x8C / 4])(
                        axisSource, nullptr, firstAxisStorage);
                void* firstAxisMatrix = CSF_AT(void*, firstAxisView, 0);

                const float deltaX = wheelX - centerX;
                const float deltaY = wheelY - centerY;
                const float deltaZ = wheelZ - centerZ;
                const float crossX = deltaY * CSF_AT(float, firstAxisMatrix, 0x8)
                                   - deltaZ * CSF_AT(float, firstAxisMatrix, 0x4);
                const float crossY = deltaZ * CSF_AT(float, firstAxisMatrix, 0)
                                   - deltaX * CSF_AT(float, firstAxisMatrix, 0x8);
                const float crossZ = deltaX * CSF_AT(float, firstAxisMatrix, 0x4)
                                   - deltaY * CSF_AT(float, firstAxisMatrix, 0);
                if (CSF_AT(std::uint8_t, firstAxisStorage, 0x4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, firstAxisStorage, 0));

                const bool positiveScale = !(crossY > g_fZero);
                const float distance = std::sqrt(
                    crossZ * crossZ + crossY * crossY + crossX * crossX);
                const float lateralOffset = distance *
                    (positiveScale ? g_ProgressScale : g_NegativeWheelScale);
                CSF_AT(float, leftWheel, 0x18) = lateralOffset;
                CSF_AT(float, rightWheel, 0x18) = lateralOffset;

                void* secondAxisSource = ((void* (__fastcall*)(void*, void*))
                    CSF_AT(void**, vehicle, 0)[0x18 / 4])(vehicle, nullptr);
                alignas(4) std::uint8_t secondAxisStorage[0x40]{};
                void* secondAxisView = ((void* (__fastcall*)(void*, void*, void*))
                    CSF_AT(void**, secondAxisSource, 0)[0x8C / 4])(
                        secondAxisSource, nullptr, secondAxisStorage);
                void* secondAxisMatrix = CSF_AT(void*, secondAxisView, 0);
                const float secondCrossX = deltaY * CSF_AT(float, secondAxisMatrix, 0x28)
                                         - deltaZ * CSF_AT(float, secondAxisMatrix, 0x24);
                const float secondCrossY = deltaZ * CSF_AT(float, secondAxisMatrix, 0x20)
                                         - deltaX * CSF_AT(float, secondAxisMatrix, 0x28);
                const float secondCrossZ = deltaX * CSF_AT(float, secondAxisMatrix, 0x24)
                                         - deltaY * CSF_AT(float, secondAxisMatrix, 0x20);
                if (CSF_AT(std::uint8_t, secondAxisStorage, 0x4))
                    RwMatrixDestroy(CSF_AT(std::uint32_t*, secondAxisStorage, 0));

                const float longitudinalOffset = std::sqrt(
                    secondCrossZ * secondCrossZ + secondCrossX * secondCrossX
                    + secondCrossY * secondCrossY) * g_NegativeWheelScale;
                CSF_AT(float, leftWheel, 0x1C) = longitudinalOffset;
                CSF_AT(float, rightWheel, 0x1C) = -longitudinalOffset;
                CSF_AT(float, leftWheel, 0x14) =
                    CSF_AT(float, leftWheel, 0x14) * self->field_0080 * g_WheelTravelMultiplier;
                CSF_AT(float, rightWheel, 0x14) =
                    CSF_AT(float, rightWheel, 0x14) * self->field_0080 * g_WheelTravelMultiplier;
            }

            ++index;
            byteOffset += 0x20;
        } while (index < self->field_003c);
    }

    return result;
}

HOOK(0x00418B40, C_0082e1e8_InitializeWheelOffsets, "auto");
