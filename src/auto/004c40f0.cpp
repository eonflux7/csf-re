#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsStateCompatible, 0x0048EF90, (void* self, void* , int requestedState));
CSF_FUNC(int*, __fastcall, S_004bd310_FillDebugRecord, 0x004BD310, (S_004bd310* self, void* , int* outRecord, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_004bd310_GetIndirectTarget, 0x004BD370, (S_004bd310* self, void* , uint32_t* output, int handle));
CSF_FUNC(uint32_t*, __fastcall, S_004bd310_FillDebugSceneHandleInfo, 0x004BD5B0, (S_004bd310* self, void* , uint32_t* outObj, int handle));
CSF_FUNC(int*, __fastcall, S_004bd310_FillHandleInfo, 0x004BD640, (S_004bd310* self, void* , int* out, int a2));

static uint32_t* __fastcall S_004bd310_BuildTargetReference(
    S_004bd310* self, void* /*edx*/, uint32_t* output,
    int source, int referenceKind)
{
    alignas(8) uint8_t handleInfoBytes[0x10];
    alignas(8) uint8_t sceneHandleInfoBytes[0x10];
    alignas(8) uint8_t indirectTargetBytes[0x10];
    alignas(8) uint8_t debugRecordBytes[0x10];
    alignas(8) uint8_t targetBytes[0x10];

    int targetKey = -1;
    int targetIndex = -1;
    CSF_AT(uint32_t, targetBytes, 0x0C) = 0;
    CSF_AT(void**, targetBytes, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, targetBytes, 0x04) = -1;
    CSF_AT(int, targetBytes, 0x08) = -1;
    CSF_AT(uint32_t, targetBytes, 0x0C) =
        CSF_AT(uint32_t, CSF_AT(void*, g_pCurrentScene, 0xB0), 0x08);

    void* filledRecord = nullptr;
    switch (referenceKind)
    {
    case 0:
        filledRecord = S_004bd310_FillDebugRecord(
            self, nullptr, reinterpret_cast<int*>(debugRecordBytes), source);
        break;
    case 1:
        filledRecord = S_004bd310_GetIndirectTarget(
            self, nullptr, reinterpret_cast<uint32_t*>(indirectTargetBytes), source);
        break;
    case 2:
        filledRecord = S_004bd310_FillDebugSceneHandleInfo(
            self, nullptr, reinterpret_cast<uint32_t*>(sceneHandleInfoBytes), source);
        break;
    case 3:
        filledRecord = S_004bd310_FillHandleInfo(
            self, nullptr, reinterpret_cast<int*>(handleInfoBytes), source);
        break;
    default:
        break;
    }

    if (static_cast<uint32_t>(referenceKind) <= 3)
    {
        targetKey = CSF_AT(int, filledRecord, 0x04);
        CSF_AT(int, targetBytes, 0x04) = targetKey;
        targetIndex = CSF_AT(int, filledRecord, 0x08);
        CSF_AT(int, targetBytes, 0x08) = targetIndex;
        CSF_AT(uint32_t, targetBytes, 0x0C) =
            CSF_AT(uint32_t, filledRecord, 0x0C);

        if (targetIndex >= 0)
        {
            const uint32_t recordCount = self->field_001c;
            bool alreadyRecorded = false;
            if (recordCount != 0)
            {
                auto* entry = static_cast<uint8_t*>(
                    CSF_AT(void*, self, 0x14)) + 4;
                uint32_t recordIndex = 0;
                while (CSF_AT(int, entry, 0x00) != targetKey)
                {
                    ++recordIndex;
                    entry += 12;
                    if (recordIndex >= recordCount)
                        break;
                }
                alreadyRecorded =
                    recordIndex < recordCount &&
                    recordIndex != static_cast<uint32_t>(-1);
            }

            if (!alreadyRecorded)
            {
                void* resolvedTarget = reinterpret_cast<void*>(
                    CHandleTable_Resolve(
                        g_pCurrentScene, nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(targetBytes))));

                if (resolvedTarget &&
                    (CSF_AT(uint32_t, resolvedTarget, 0x170) & 0x2000) != 0 &&
                    static_cast<uint8_t>(IsStateCompatible(
                        reinterpret_cast<void*>(self->field_0000), nullptr,
                        CSF_AT(int, resolvedTarget, 0x78))) != 0)
                {
                    if (self->field_001c >= CSF_AT(uint32_t, self, 0x18))
                    {
                        S_00413570_GrowEventRecords(
                            reinterpret_cast<S_00413570*>(
                                reinterpret_cast<uint8_t*>(self) + 0x14),
                            nullptr, CSF_AT(int, self, 0x20));
                    }

                    const uint32_t recordIndex = self->field_001c;
                    auto* entry = static_cast<uint8_t*>(
                        CSF_AT(void*, self, 0x14)) + 12 * recordIndex;
                    self->field_001c = recordIndex + 1;
                    CSF_AT(int, entry, 0x04) = CSF_AT(int, targetBytes, 0x04);
                    CSF_AT(int, entry, 0x08) = CSF_AT(int, targetBytes, 0x08);

                    S_004bd310_BuildTargetReference(
                        self, nullptr,
                        reinterpret_cast<uint32_t*>(debugRecordBytes),
                        static_cast<int>(reinterpret_cast<uintptr_t>(targetBytes)), 1);

                    if (CSF_AT(int, debugRecordBytes, 0x08) >= 0)
                    {
                        targetKey = CSF_AT(int, debugRecordBytes, 0x04);
                        CSF_AT(int, targetBytes, 0x08) =
                            CSF_AT(int, debugRecordBytes, 0x08);
                        CSF_AT(int, targetBytes, 0x04) = targetKey;
                        CSF_AT(uint32_t, targetBytes, 0x0C) =
                            CSF_AT(uint32_t, debugRecordBytes, 0x0C);
                    }
                }
                targetKey = CSF_AT(int, targetBytes, 0x04);
            }

            bool rejectTarget = targetKey == CSF_AT(
                int, reinterpret_cast<void*>(self->field_0000), 0x20);

            if (!rejectTarget)
            {
                void* resolvedTarget = reinterpret_cast<void*>(
                    CHandleTable_Resolve(
                        g_pCurrentScene, nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(targetBytes))));

                rejectTarget = !resolvedTarget;
                if (resolvedTarget &&
                    (CSF_AT(uint32_t, resolvedTarget, 0x170) & 0x2000) != 0)
                {
                    rejectTarget = static_cast<uint8_t>(IsStateCompatible(
                        reinterpret_cast<void*>(self->field_0000), nullptr,
                        CSF_AT(int, resolvedTarget, 0x78))) != 0;
                }
            }

            if (rejectTarget)
            {
                targetKey = -1;
                targetIndex = -1;
            }
            else
            {
                targetIndex = CSF_AT(int, targetBytes, 0x08);
                targetKey = CSF_AT(int, targetBytes, 0x04);
            }
        }
    }

    output[2] = static_cast<uint32_t>(targetIndex);
    const uint32_t sceneId = CSF_AT(uint32_t, targetBytes, 0x0C);
    CSF_AT(void**, output, 0x00) = CSF_VTBL(0x0082D448);
    output[1] = static_cast<uint32_t>(targetKey);
    output[3] = sceneId;
    return output;
}

HOOK(0x004C40F0, S_004bd310_BuildTargetReference, "auto");
