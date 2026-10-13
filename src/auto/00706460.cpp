#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_00861d48_LoadFromRecord, 0x006E6AE0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(char, __fastcall, C_00861d48_GrowEntryBuffer, 0x006E6B80, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, Bounds_ConstructFromHalfExtents, 0x00706170, (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5));
CSF_FUNC(uint32_t*, __fastcall, C_00863760_CopyFromDwordArray, 0x0070BAC0, (C_00863760* self, void* , uint32_t* a2));
CSF_FUNC(C_0086379c*, __fastcall, C_0086379c_Construct, 0x0070BD90, (C_0086379c* self, void* , const uint32_t* a2));
CSF_FUNC(C_008637d8*, __fastcall, C_008637d8_ConstructFromValues, 0x0070C300, (C_008637d8* self, void* , uint32_t* sourceValues));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_00863260_LoadCollisionShapes(
    C_00863260* self, void* /*edx*/, int recordId, int recordAddress)
{
    auto* record = reinterpret_cast<uint32_t*>(recordAddress);

    alignas(8) uint8_t boundsBytes[0x18];
    alignas(8) uint8_t boxValuesBytes[0x18];
    alignas(8) uint8_t sphereValuesBytes[0x10];
    alignas(8) uint8_t fallbackValuesBytes[0x10];
    auto* boxValues = reinterpret_cast<uint32_t*>(boxValuesBytes);

    C_00861d48_LoadFromRecord(self, nullptr, recordId, record);

    const uint32_t shapeType = CSF_AT(uint32_t, record, 0x28);
    if (shapeType == 0)
    {
        float* largest = reinterpret_cast<float*>(
            reinterpret_cast<uint8_t*>(record) + 8);
        if (!(CSF_AT(float, record, 4) < *largest))
            largest = reinterpret_cast<float*>(
                reinterpret_cast<uint8_t*>(record) + 4);
        if (!(CSF_AT(float, record, 0) < *largest))
            largest = reinterpret_cast<float*>(record);

        const float largestExtent = *largest;
        auto* sphere = reinterpret_cast<C_00863760*>(
            CoreMemory_AllocUntagged(24));
        uint32_t* shape = nullptr;
        if (sphere)
        {
            CSF_AT(float, sphereValuesBytes, 0) = g_ZeroVectorX;
            CSF_AT(float, sphereValuesBytes, 4) = g_ZeroVectorY;
            CSF_AT(float, sphereValuesBytes, 12) =
                largestExtent * g_HalfDurationScale;
            CSF_AT(float, sphereValuesBytes, 8) = g_ZeroVectorZ;
            shape = C_00863760_CopyFromDwordArray(
                sphere, nullptr, reinterpret_cast<uint32_t*>(sphereValuesBytes));
        }
        CSF_AT(uint32_t, self, 0x24) = reinterpret_cast<uint32_t>(shape);
    }
    else if (shapeType == 1)
    {
        auto* box = reinterpret_cast<C_0086379c*>(
            CoreMemory_AllocUntagged(32));
        C_0086379c* shape = nullptr;
        if (box)
        {
            const float halfZ = CSF_AT(float, record, 8) * g_HalfDurationScale;
            const float halfY = CSF_AT(float, record, 4) * g_HalfDurationScale;
            const float halfX = CSF_AT(float, record, 0) * g_HalfDurationScale;
            int xArgument, yArgument, zArgument;
            std::memcpy(&xArgument, &halfX, sizeof(xArgument));
            std::memcpy(&yArgument, &halfY, sizeof(yArgument));
            std::memcpy(&zArgument, &halfZ, sizeof(zArgument));

            uint32_t* bounds = Bounds_ConstructFromHalfExtents(
                boundsBytes, nullptr, xArgument, yArgument, zArgument,
                reinterpret_cast<uint32_t*>(&g_ZeroVectorX));
            shape = C_0086379c_Construct(box, nullptr, bounds);
        }
        CSF_AT(uint32_t, self, 0x24) = reinterpret_cast<uint32_t>(shape);
    }
    else if (shapeType == 2)
    {
        auto* box = reinterpret_cast<C_008637d8*>(
            CoreMemory_AllocUntagged(40));
        C_008637d8* shape = nullptr;
        if (box)
        {
            boxValues[3] = 0;
            boxValues[4] = 0;
            boxValues[5] = 0;
            boxValues[0] = 0;
            boxValues[1] = 0;
            boxValues[2] = 0;
            shape = C_008637d8_ConstructFromValues(box, nullptr, boxValues);
        }
        CSF_AT(uint32_t, self, 0x24) = reinterpret_cast<uint32_t>(shape);
        shape->vSlot18(reinterpret_cast<float*>(record));
    }
    else
    {
        auto* sphere = reinterpret_cast<C_00863760*>(
            CoreMemory_AllocUntagged(24));
        uint32_t* shape = nullptr;
        if (sphere)
        {
            CSF_AT(float, fallbackValuesBytes, 0) = g_ZeroVectorX;
            const float radius =
                CSF_AT(float, record, 4) * g_HalfDurationScale;
            CSF_AT(float, fallbackValuesBytes, 4) = g_ZeroVectorY;
            CSF_AT(float, fallbackValuesBytes, 12) = radius;
            CSF_AT(float, fallbackValuesBytes, 8) = g_ZeroVectorZ;
            shape = C_00863760_CopyFromDwordArray(
                sphere, nullptr, reinterpret_cast<uint32_t*>(fallbackValuesBytes));
        }
        CSF_AT(uint32_t, self, 0x24) = reinterpret_cast<uint32_t>(shape);
    }

    auto* box = reinterpret_cast<C_008637d8*>(
        CoreMemory_AllocUntagged(40));
    C_008637d8* entryShape = nullptr;
    if (box)
    {
        boxValues[3] = 0;
        boxValues[4] = 0;
        boxValues[5] = 0;
        boxValues[0] = 0;
        boxValues[1] = 0;
        boxValues[2] = 0;
        entryShape = C_008637d8_ConstructFromValues(box, nullptr, boxValues);
    }
    entryShape->vSlot18(reinterpret_cast<float*>(record));

    void* entries = reinterpret_cast<uint8_t*>(self) + 0x28;
    if (CSF_AT(uint32_t, self, 0x2C) < 1)
        C_00861d48_GrowEntryBuffer(entries, nullptr, 1);
    if (CSF_AT(uint32_t, self, 0x30) >= CSF_AT(uint32_t, self, 0x2C))
        C_00861d48_GrowEntryBuffer(
            entries, nullptr, CSF_AT(int, self, 0x34));

    const uint32_t count = CSF_AT(uint32_t, self, 0x30);
    const uint32_t entryAddress =
        CSF_AT(uint32_t, self, 0x28) + 16 * count;
    CSF_AT(uint32_t, self, 0x30) = count + 1;

    auto* entry = reinterpret_cast<void*>(entryAddress);
    CSF_AT(uint32_t, entry, 0) = reinterpret_cast<uint32_t>(entryShape);
    CSF_AT(int, entry, 4) = -1;
    CSF_AT(int, entry, 8) = -1;
    CSF_AT(uint32_t, entry, 12) = 0xFFFFFFFF;
    CSF_AT(uint8_t, self, 0x38) = 0;
    CSF_AT(uint8_t, self, 0x39) = 0;
    return static_cast<int>(entryAddress);
}

HOOK(0x00706460, C_00863260_LoadCollisionShapes, "auto");
