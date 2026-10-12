#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082e0f4_InitializeSpawnRecord, 0x004164B0, (C_0082e0f4* self, void* , int ownerId));
CSF_FUNC(int, __fastcall, CGuiControl_ProcessDisplayFlags_453070, 0x00453070, (CGuiControl* self, void* , void* a2, float* a3, int* a4, int a5, float a6));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008342e8_RefreshDepletedStatus(
    C_008342e8* self, void* /*edx*/)
{
    alignas(8) uint8_t filteredDirectionBytes[0xC];
    alignas(8) uint8_t unrestrictedDirectionBytes[0xC];
    alignas(8) uint8_t recordBytes[0x74];
    alignas(8) uint8_t notificationBytes[0x58];
    alignas(8) uint8_t queryOptionsBytes[0x34];
    alignas(8) uint8_t querySphereBytes[0x10];
    alignas(8) uint8_t queryFlagBytes[0x4];
    alignas(8) uint8_t statusFlagBytes[0x8];
    alignas(8) uint8_t queryResultsBytes[0x14];
    alignas(8) uint8_t offsetBytes[0xC];
    alignas(8) uint8_t centerBytes[0x10];

    auto& record = *reinterpret_cast<C_0082e0f4*>(recordBytes);
    auto* filteredDirection =
        reinterpret_cast<uint32_t*>(filteredDirectionBytes);
    auto* unrestrictedDirection =
        reinterpret_cast<uint32_t*>(unrestrictedDirectionBytes);
    auto* offset = reinterpret_cast<float*>(offsetBytes);
    auto* center = reinterpret_cast<float*>(centerBytes);
    auto* querySphere = reinterpret_cast<float*>(querySphereBytes);
    auto* queryOptions = reinterpret_cast<int*>(queryOptionsBytes);

    void* globalObject = reinterpret_cast<void*>(GetValue8B0554());
    void* definitionTable = CSF_AT(void*, globalObject, 0x1FC0);
    int definitionAddress =
        reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, definitionTable, 0)[8 / 4])(
                definitionTable, nullptr, CSF_AT(uint32_t, self, 0x194));
    auto* definition = reinterpret_cast<uint8_t*>(definitionAddress) + 4;

    offset[0] = reinterpret_cast<const float&>(self->field_019c)
              * g_MessageBoxAnimationScale;
    offset[1] = reinterpret_cast<const float&>(self->field_01a0)
              * g_MessageBoxAnimationScale;
    offset[2] = reinterpret_cast<const float&>(self->field_01a4)
              * g_MessageBoxAnimationScale;

    float* position = GetObjectPositionVector(self, nullptr);
    center[0] = offset[0] + position[0];
    center[1] = offset[1] + position[1];
    center[2] = offset[2] + position[2];

    C_0082e0f4_InitializeSpawnRecord(&record, nullptr, 4);
    uint32_t ownerId = CSF_AT(uint32_t, self, 0x18C);
    record.field_0010 = CSF_AT(uint32_t, self, 0x190);
    record.field_000c = ownerId;

    position = GetObjectPositionVector(self, nullptr);
    record.field_0034 = CSF_AT(uint32_t, position, 0);
    record.field_0038 = CSF_AT(uint32_t, position, 4);
    record.field_003c = CSF_AT(uint32_t, position, 8);
    record.field_0060 = CSF_AT(uint32_t, definition, 0x70);
    record.field_006c = CSF_AT(uint32_t, definition, 0x114);
    record.field_0071 = 1;

    querySphere[1] = center[1];
    querySphere[3] = CSF_AT(float, definition, 0x120);
    CSF_AT(int, queryResultsBytes, 0xC) = 10;
    queryOptions[7] = 10;
    querySphere[0] = center[0];
    querySphere[2] = center[2];

    CSF_AT(uint32_t, queryResultsBytes, 0) = 0;
    CSF_AT(uint32_t, queryResultsBytes, 4) = 0;
    CSF_AT(uint32_t, queryResultsBytes, 8) = 0;
    queryOptions[4] = 0;
    queryOptions[5] = 0;
    queryOptions[6] = 0;
    queryOptions[8] = 0;
    queryOptions[9] = 0;
    queryOptions[10] = 0;
    std::memset(&queryOptions[1], 255, 12);
    queryOptions[11] = -1;
    queryOptions[12] = 0;
    queryOptions[0] = 273;
    CSF_AT(int, queryFlagBytes, 0) = 1;

    CGuiControl_ProcessDisplayFlags_453070(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr,
        queryResultsBytes, querySphere, queryOptions,
        reinterpret_cast<int>(queryFlagBytes), 0.0f);

    CSF_AT(uint32_t, statusFlagBytes, 4) =
        CSF_AT(uint32_t, queryResultsBytes, 8);
    CSF_AT(uint32_t, queryResultsBytes, 0x10) = 0;

    while (CSF_AT(uint32_t, queryResultsBytes, 0x10)
           < CSF_AT(uint32_t, statusFlagBytes, 4))
    {
        auto* targets = reinterpret_cast<C_00833908**>(
            CSF_AT(uint32_t, queryResultsBytes, 0));
        C_00833908* target =
            targets[CSF_AT(uint32_t, queryResultsBytes, 0x10)];

        switch (self->field_0050)
        {
        case 0xE:
            if ((CSF_AT(uint8_t, target, 0x1F0) & 2) == 0)
            {
                record.field_0018 = target->field_0020;
                record.field_001c = target->field_0024;
                float* sourcePosition = GetObjectPositionVector(self, nullptr);
                float* targetPosition = GetObjectPositionVector(target, nullptr);
                float dz = targetPosition[2] - sourcePosition[2];
                float dy = targetPosition[1] - sourcePosition[1];
                offset[0] = targetPosition[0] - sourcePosition[0];
                offset[1] = dy;
                offset[2] = dz;
                uint32_t* direction = S_00526d40_ConvertDirection(
                    reinterpret_cast<S_00526d40*>(offsetBytes), nullptr,
                    filteredDirection);
                record.field_004c = direction[0];
                record.field_0050 = direction[1];
                record.field_0054 = direction[2];

                void* dispatcher =
                    reinterpret_cast<void*>(GetGlobalObjectField184());
                DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1);

                uint32_t effectId = CSF_AT(uint32_t, definition, 0x114);
                uint32_t effectType = CSF_AT(uint32_t, definition, 0x70);
                void* recipient = reinterpret_cast<void*>(target->field_0038);
                auto* message = notificationBytes;
                std::memset(message + 0x14, 0, 24);
                uint32_t ownerGeneration = CSF_AT(uint32_t, self, 0x190);
                CSF_AT(uint32_t, message, 0x10) = effectId;
                uint32_t currentOwnerId = CSF_AT(uint32_t, self, 0x18C);
                CSF_AT(uint32_t, message, 8) = ownerGeneration;
                CSF_AT(uint32_t, message, 0xC) = effectType;
                CSF_AT(void**, message, 0) = CSF_VTBL(0x0082D448);
                CSF_AT(uint32_t, message, 4) = currentOwnerId;
                reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                    CSF_AT(void**, recipient, 0)[0x8C / 4])(
                        recipient, nullptr, message);
                CSF_AT(void**, message, 0) = CSF_VTBL(0x0082D410);
            }
            break;

        case 0xF:
            if ((CSF_AT(uint8_t, target, 0x1F0) & 2) == 0)
            {
                void* statusObject = reinterpret_cast<uint8_t*>(target) + 4;
                CSF_AT(int, statusFlagBytes, 0) = 0x40000000;
                reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
                    CSF_AT(void**, statusObject, 0)[0x2C / 4])(
                        statusObject, nullptr,
                        reinterpret_cast<int*>(statusFlagBytes));
            }
            break;

        case 0x10:
        {
            record.field_0018 = target->field_0020;
            record.field_001c = target->field_0024;
            float* sourcePosition = GetObjectPositionVector(self, nullptr);
            float* targetPosition = GetObjectPositionVector(target, nullptr);
            float dz = targetPosition[2] - sourcePosition[2];
            float dy = targetPosition[1] - sourcePosition[1];
            center[0] = targetPosition[0] - sourcePosition[0];
            center[1] = dy;
            center[2] = dz;
            uint32_t* direction = S_00526d40_ConvertDirection(
                reinterpret_cast<S_00526d40*>(centerBytes), nullptr,
                unrestrictedDirection);
            record.field_004c = direction[0];
            record.field_0050 = direction[1];
            record.field_0054 = direction[2];

            void* dispatcher =
                reinterpret_cast<void*>(GetGlobalObjectField184());
            DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1);

            uint32_t effectType = CSF_AT(uint32_t, definition, 0x70);
            void* recipient = reinterpret_cast<void*>(target->field_0038);
            auto* message = notificationBytes + 0x2C;
            std::memset(message + 0x14, 0, 20);
            CSF_AT(int, message, 4) = -1;
            CSF_AT(int, message, 8) = -1;
            uint32_t effectId = CSF_AT(uint32_t, definition, 0x114);
            CSF_AT(void**, message, 0) = CSF_VTBL(0x0082D448);
            CSF_AT(uint32_t, message, 0x10) = effectId;
            CSF_AT(uint32_t, message, 0xC) = effectType;
            CSF_AT(uint32_t, message, 0x28) = effectId;
            reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, recipient, 0)[0x8C / 4])(
                    recipient, nullptr, message);
            CSF_AT(void**, message, 0) = CSF_VTBL(0x0082D410);
            break;
        }
        }

        ++CSF_AT(uint32_t, queryResultsBytes, 0x10);
    }

    if (queryOptions[4])
    {
        Mem_Free(reinterpret_cast<void*>(queryOptions[4]));
        queryOptions[4] = 0;
    }

    uint32_t result = CSF_AT(uint32_t, queryResultsBytes, 0);
    if (result)
        return Mem_Free(reinterpret_cast<void*>(result));
    return static_cast<int>(result);
}

HOOK(0x0049DA70, C_008342e8_RefreshDepletedStatus, "auto");
