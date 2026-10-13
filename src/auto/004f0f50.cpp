#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, ResetPendingStateAndGetInnerBlockAddress, 0x00445F20, (void* self, void*));
CSF_FUNC(int, __fastcall, SpatialQuery_CollectEntriesNearObject, 0x00447040, (void* self, void* , int handle, S_00446450* results));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_004f0cd0_AcceptNearbyEntry, 0x004F0CD0, (void* self, void* edx, uint32_t* a2));

static uint8_t* __fastcall S_004f0cd0_ProcessNearbyEntries(
    S_004f0cd0* self, void* /*edx*/)
{
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        reinterpret_cast<void**>(self->field_0000)[0x8C / 4])(
            self, nullptr);

    auto* results = reinterpret_cast<S_00446450*>(
        ResetPendingStateAndGetInnerBlockAddress(
            CSF_AT(void*, g_pCurrentScene, 0xB0), nullptr));

    const int queryResult = SpatialQuery_CollectEntriesNearObject(
        CSF_AT(void*, g_pCurrentScene, 0xB0), nullptr,
        reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(self->field_0008) + 0x1C),
        results);

    if (static_cast<uint8_t>(queryResult) == 0)
    {
        auto* scene = reinterpret_cast<uint8_t*>(g_pCurrentScene);
        CSF_AT(uint8_t, CSF_AT(void*, scene, 0xB0), 0xFC) = 0;
        return scene;
    }

    alignas(8) uint8_t entryBytes[0x30];
    CSF_AT(float*, entryBytes, 0x28) = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0008), nullptr);

    const uint32_t entryCount = results->field_0008;
    if (entryCount == 0)
    {
        CSF_AT(uint8_t, CSF_AT(void*, g_pCurrentScene, 0xB0), 0xFC) = 0;
        return nullptr;
    }

    CSF_AT(uint32_t, entryBytes, 0x2C) = entryCount;
    uint32_t entryOffset = 0;

    do
    {
        auto* source = reinterpret_cast<uint8_t*>(results->field_0000)
                     + entryOffset;
        auto* object = reinterpret_cast<C_008342e8*>(self->field_0008);

        CSF_AT(int, entryBytes, 0x04) = -1;
        CSF_AT(int, entryBytes, 0x0C) = -1;
        CSF_AT(int, entryBytes, 0x10) = -1;
        CSF_AT(int, entryBytes, 0x00) = -1;
        CSF_AT(void**, entryBytes, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(float, entryBytes, 0x14) = 0.0f;
        CSF_AT(float, entryBytes, 0x18) = 0.0f;
        CSF_AT(float, entryBytes, 0x1C) = 0.0f;
        CSF_AT(float, entryBytes, 0x20) = 3.4028235e38f;
        CSF_AT(int, entryBytes, 0x24) = 0;

        CSF_AT(int, entryBytes, 0x04) = CSF_AT(int, source, 0x00);
        const uint32_t sourceObject = CSF_AT(uint32_t, source, 0x0C);
        if (sourceObject != object->field_0020)
        {
            CSF_AT(uint32_t, entryBytes, 0x0C) = sourceObject;
            CSF_AT(uint32_t, entryBytes, 0x10) =
                CSF_AT(uint32_t, source, 0x10);

            if (static_cast<uint8_t>(S_004f0cd0_AcceptNearbyEntry(
                    self, nullptr, reinterpret_cast<uint32_t*>(entryBytes))) != 0)
            {
                CSF_AT(uint32_t, entryBytes, 0x24) =
                    CSF_AT(uint32_t,
                        CSF_AT(void*, g_pCurrentScene, 0xB0), 0x08);

                auto* position = reinterpret_cast<uint8_t*>(results->field_0000)
                               + entryOffset + 0x14;
                CSF_AT(float, entryBytes, 0x14) =
                    CSF_AT(float, position, 0x00);

                float* objectPosition = CSF_AT(float*, entryBytes, 0x28);
                const float dx =
                    objectPosition[0] - CSF_AT(float, entryBytes, 0x14);

                CSF_AT(float, entryBytes, 0x18) =
                    CSF_AT(float, position, 0x04);
                const float dy =
                    objectPosition[1] - CSF_AT(float, entryBytes, 0x18);

                CSF_AT(float, entryBytes, 0x1C) =
                    CSF_AT(float, position, 0x08);
                const float dz =
                    objectPosition[2] - CSF_AT(float, entryBytes, 0x1C);

                CSF_AT(float, entryBytes, 0x20) =
                    dz * dz + dy * dy + dx * dx;

                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
                    reinterpret_cast<void**>(self->field_0000)[0x90 / 4])(
                        self, nullptr, reinterpret_cast<uint32_t*>(entryBytes));
            }
        }

        const uint32_t remaining = CSF_AT(uint32_t, entryBytes, 0x2C) - 1;
        entryOffset += 40;
        CSF_AT(void**, entryBytes, 0x08) = CSF_VTBL(0x0082D410);
        CSF_AT(uint32_t, entryBytes, 0x2C) = remaining;
    }
    while (CSF_AT(uint32_t, entryBytes, 0x2C) != 0);

    auto* queryState = CSF_AT(uint8_t*, g_pCurrentScene, 0xB0);
    CSF_AT(uint8_t, queryState, 0xFC) = 0;
    return queryState;
}

HOOK(0x004F0F50, S_004f0cd0_ProcessNearbyEntries, "auto");
