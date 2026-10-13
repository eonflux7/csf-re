#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneMissionSetup_FindNamedEntry, 0x006D03B0, (char* self, void* , const char* name));
CSF_FUNC(int, __fastcall, C_00861d48_LoadFromRecord, 0x006E6AE0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __fastcall, Bounds_ConstructFromHalfExtents, 0x00706170, (void* self, void* edx, int a2, int a3, int a4, uint32_t* a5));
CSF_FUNC(int, __fastcall, C_008632c0_ApplyNamedEntry, 0x00706E80, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00863760_CopyFromDwordArray, 0x0070BAC0, (C_00863760* self, void* , uint32_t* a2));
CSF_FUNC(C_0086379c*, __fastcall, C_0086379c_Construct, 0x0070BD90, (C_0086379c* self, void* , const uint32_t* a2));
CSF_FUNC(C_008637d8*, __fastcall, C_008637d8_ConstructFromValues, 0x0070C300, (C_008637d8* self, void* , uint32_t* sourceValues));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// The original callee's integer parameters carry float representations.
static int Bounds_PackFloatArgument(float value)
{
    int argument;
    std::memcpy(&argument, &value, sizeof(argument));
    return argument;
}

static uint32_t* __fastcall C_008632c0_LoadFromRecord(
    C_008632c0* self, void* /*edx*/, int owner, uint32_t* record)
{
    C_00861d48_LoadFromRecord(self, nullptr, owner, record);

    switch (CSF_AT(uint32_t, record, 0x28))
    {
    case 0:
    {
        float* largest = &CSF_AT(float, record, 0x8);
        if (!(CSF_AT(float, record, 0x4) < *largest))
            largest = &CSF_AT(float, record, 0x4);
        if (!(CSF_AT(float, record, 0x0) < *largest))
            largest = &CSF_AT(float, record, 0x0);

        const float diameter = *largest;
        auto* sphere = reinterpret_cast<C_00863760*>(
            CoreMemory_AllocUntagged(24));
        uint32_t* shape = nullptr;
        if (sphere)
        {
            alignas(8) uint8_t sphereValues[0x10];
            CSF_AT(float, sphereValues, 0x0) = g_ZeroVectorX;
            CSF_AT(float, sphereValues, 0x4) = g_ZeroVectorY;
            CSF_AT(float, sphereValues, 0xC) =
                diameter * g_HalfDurationScale;
            CSF_AT(float, sphereValues, 0x8) = g_ZeroVectorZ;
            shape = C_00863760_CopyFromDwordArray(
                sphere, nullptr, reinterpret_cast<uint32_t*>(sphereValues));
        }
        self->field_0024 = reinterpret_cast<uint32_t>(shape);
        break;
    }

    case 1:
    {
        auto* box = reinterpret_cast<C_0086379c*>(
            CoreMemory_AllocUntagged(32));
        C_0086379c* shape = nullptr;
        if (box)
        {
            const int halfDepth = Bounds_PackFloatArgument(
                CSF_AT(float, record, 0x8) * g_HalfDurationScale);
            const int halfHeight = Bounds_PackFloatArgument(
                CSF_AT(float, record, 0x4) * g_HalfDurationScale);
            const int halfWidth = Bounds_PackFloatArgument(
                CSF_AT(float, record, 0x0) * g_HalfDurationScale);

            alignas(8) uint8_t boundsBytes[0x18];
            uint32_t* bounds = Bounds_ConstructFromHalfExtents(
                boundsBytes, nullptr, halfWidth, halfHeight, halfDepth,
                reinterpret_cast<uint32_t*>(&g_ZeroVectorX));
            shape = C_0086379c_Construct(box, nullptr, bounds);
        }
        self->field_0024 = reinterpret_cast<uint32_t>(shape);
        break;
    }

    case 2:
    {
        auto* allocatedShape = reinterpret_cast<C_008637d8*>(
            CoreMemory_AllocUntagged(40));
        void* shape = nullptr;
        if (allocatedShape)
        {
            alignas(8) uint8_t valuesBytes[0x18];
            std::memset(valuesBytes, 0, sizeof(valuesBytes));
            shape = C_008637d8_ConstructFromValues(
                allocatedShape, nullptr,
                reinterpret_cast<uint32_t*>(valuesBytes));
        }
        self->field_0024 = reinterpret_cast<uint32_t>(shape);

        // The original also dispatches through null on allocation failure.
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, shape, 0x0)[0x18 / 4])(
                shape, nullptr, record);
        break;
    }
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(globalObject, nullptr));

    const char* name;
    if (CSF_AT(uint32_t, record, 0x24) < 0x10u)
        name = reinterpret_cast<const char*>(record) + 0x10;
    else
        name = reinterpret_cast<const char*>(
            CSF_AT(uint32_t, record, 0x10));

    char* missionSetup =
        reinterpret_cast<char*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0x0)[0x18 / 4])(
                accessor, nullptr);
    auto* entry = reinterpret_cast<uint32_t*>(
        SceneMissionSetup_FindNamedEntry(missionSetup, nullptr, name));
    self->field_0050 = reinterpret_cast<uint32_t>(entry);
    C_008632c0_ApplyNamedEntry(self, nullptr, entry);

    uint32_t* result = nullptr;
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0050), 0x38))
    {
        result = reinterpret_cast<uint32_t*>(CoreMemory_AllocUntagged(12));
        if (result)
        {
            result[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D428));
            result[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0086333C));
            result[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x00863320));
            result[2] = static_cast<uint32_t>(owner);
        }
    }

    self->field_004c = reinterpret_cast<uint32_t>(result);
    self->field_0038 = 0;
    self->field_0039 = 0;
    return result;
}

HOOK(0x00707220, C_008632c0_LoadFromRecord, "auto");
