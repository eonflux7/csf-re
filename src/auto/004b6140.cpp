#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using std::uint32_t;

CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverAttackTasks_GrowRecordArray, 0x004ADE60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(int, __fastcall, GfxMatrix_SetScaleAndUpdate, 0x006AD9B0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisY, 0x006AE1D0, (int* self, void* , float angle));

CSF_GLOBAL(float, g_fReturnedConstant_0082E030, 0x0082E030);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00835440);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static char __fastcall AiDistracted_InitializeCoreActionState(
    void* self, void* /*edx*/, void* linkedObject)
{
    CSF_AT(uint32_t, self, 0x04) = 1;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;

    int linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    void* linkedBase = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    if (CSF_AT(int, linkedBase, 0x608) == -1 &&
        CSF_AT(int, linkedBase, 0x60C) == -1)
    {
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(linkedBase));
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    linkedBase = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    float* position = GetObjectPositionVector(
        CSF_AT(void*, linkedBase, 0x2C), nullptr);
    const float raisedY = position[1] + g_fReturnedConstant_0082E030;
    const uint32_t positionX = CSF_AT(uint32_t, position, 0x00);
    const uint32_t positionZ = CSF_AT(uint32_t, position, 0x08);

    void* records = static_cast<std::uint8_t*>(self) + 0x14;
    const uint32_t capacity = CSF_AT(uint32_t, records, 0x04);
    if (capacity < 7)
        CoverAttackTasks_GrowRecordArray(records, nullptr, 7 - capacity);

    uint32_t record[7];
    record[0] = positionX;
    CSF_AT(float, record, 0x04) = raisedY;
    record[2] = positionZ;
    CSF_AT(float, record, 0x0C) = g_ZeroVectorX;
    CSF_AT(float, record, 0x10) = g_ZeroVectorY;
    CSF_AT(float, record, 0x14) = g_ZeroVectorZ;
    record[6] = 0;

    if (CSF_AT(uint32_t, records, 0x08) >=
        CSF_AT(uint32_t, records, 0x04))
    {
        CoverAttackTasks_GrowRecordArray(
            records, nullptr, CSF_AT(int, records, 0x0C));
    }

    uint32_t count = CSF_AT(uint32_t, records, 0x08);
    auto* destination = CSF_AT(std::uint8_t*, records, 0x00) + 0x1C * count;
    CSF_AT(uint32_t, records, 0x08) = count + 1;
    std::memcpy(destination, record, 0x1C);

    linkedValue = GetLinkedObjectValue_4DB5C0(linkedObject, nullptr);
    linkedBase = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    alignas(8) std::uint8_t directionBytes[0x24];
    const float directionZ = CSF_AT(float, linkedBase, 0x654) - position[2];
    const float directionX = CSF_AT(float, linkedBase, 0x64C) - position[0];
    CSF_AT(float, directionBytes, 0x04) = 0.0f;
    CSF_AT(float, directionBytes, 0x00) = directionX;
    CSF_AT(float, directionBytes, 0x08) = directionZ;
    NormalizeVector3(reinterpret_cast<C_00833b88*>(directionBytes), nullptr);

    CSF_AT(float, directionBytes, 0x0C) = 90.0f;
    CSF_AT(float, directionBytes, 0x10) = -90.0f;
    CSF_AT(float, directionBytes, 0x14) = 135.0f;
    CSF_AT(float, directionBytes, 0x18) = 45.0f;
    CSF_AT(float, directionBytes, 0x1C) = -45.0f;
    CSF_AT(float, directionBytes, 0x20) = -135.0f;
    CSF_AT(float, self, 0x28) = 100.0f;

    alignas(8) std::uint8_t matrixBytes[0x10] = {};
    CSF_AT(uint32_t*, matrixBytes, 0x00) = RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 0x04) = 1;

    for (uint32_t angleIndex = 0; angleIndex < 6; ++angleIndex)
    {
        GfxMatrix_SetScaleAndUpdate(
            matrixBytes, nullptr, reinterpret_cast<const uint32_t*>(directionBytes));
        const float angle =
            CSF_AT(float, directionBytes, 0x0C + angleIndex * 4) *
            g_DegreesToRadians;
        GfxMatrix_RotateAroundAxisY(
            reinterpret_cast<int*>(matrixBytes), nullptr, angle);

        uint32_t* matrix = CSF_AT(uint32_t*, matrixBytes, 0x00);
        record[3] = CSF_AT(uint32_t, matrix, 0x20);
        record[4] = CSF_AT(uint32_t, matrix, 0x24);
        record[5] = CSF_AT(uint32_t, matrix, 0x28);
        record[6] = CSF_AT(uint32_t, self, 0x28);

        if (CSF_AT(uint32_t, records, 0x08) >=
            CSF_AT(uint32_t, records, 0x04))
        {
            CoverAttackTasks_GrowRecordArray(
                records, nullptr, CSF_AT(int, records, 0x0C));
        }

        count = CSF_AT(uint32_t, records, 0x08);
        destination = CSF_AT(std::uint8_t*, records, 0x00) + 0x1C * count;
        CSF_AT(uint32_t, records, 0x08) = count + 1;
        std::memcpy(destination, record, 0x1C);
    }

    const char ownsMatrix = CSF_AT(char, matrixBytes, 0x04);
    if (ownsMatrix)
        return static_cast<char>(
            RwMatrixDestroy(CSF_AT(uint32_t*, matrixBytes, 0x00)));
    return ownsMatrix;
}

HOOK(0x004B6140, AiDistracted_InitializeCoreActionState, "auto");
