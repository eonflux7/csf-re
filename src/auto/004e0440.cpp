#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_004de770_LockAndAddRef, 0x004DE770, (void* self, void*));
CSF_FUNC(void, __fastcall, C_004de770_UnlockAndRelease, 0x004DE780, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, PathSearch_GrowRecords, 0x004DF5B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_004e0130_PopNextIndex, 0x004DFE00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_004e0130_InsertIndex, 0x004E01A0, (void* self, void* edx, uint32_t* a6, int a3));
CSF_FUNC(char, __fastcall, S_004dfbc0_ExpandRecord, 0x004E0200, (void* self, void* edx, int arg0, float* a3, uint32_t* a2, void* a5));
CSF_FUNC(int, __fastcall, PathSearch_GetNodeData, 0x004E08F0, (void* self, void* edx));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall S_004dfbc0_FindPathRecord(
    S_004dfbc0* self, void* /*edx*/, int startNode, int targetNode,
    float* startPosition, float* targetPosition, char flags,
    uint32_t* records)
{
    auto* frontier = reinterpret_cast<S_004e0130*>(
        reinterpret_cast<uint8_t*>(self) + 0x1C);

    int currentRecord = 0;
    int closestRecord = 0;
    float closestDistance = 100000000.0f;

    if (self->field_0020)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0020));

    frontier->field_0004 = 0;
    frontier->field_0008 = 0;
    frontier->field_000c = 0;

    C_004de770_LockAndAddRef(
        reinterpret_cast<void*>(self->field_0004), nullptr);

    uint32_t initialRecord[9];
    initialRecord[1] = static_cast<uint32_t>(startNode);
    CSF_AT(float, initialRecord, 0x0C) = startPosition[0];
    CSF_AT(float, initialRecord, 0x10) = startPosition[1];
    CSF_AT(float, initialRecord, 0x14) = startPosition[2];

    alignas(8) uint8_t directionBytes[0x1C];
    auto* direction = reinterpret_cast<float*>(directionBytes);
    direction[0] = targetPosition[0] - startPosition[0];
    initialRecord[0] = 0;
    initialRecord[6] = 0;
    initialRecord[2] = 0;
    initialRecord[7] = 0;
    direction[1] = targetPosition[1] - startPosition[1];
    direction[2] = targetPosition[2] - startPosition[2];

    CSF_AT(float, initialRecord, 0x20) =
        static_cast<float>(RwV3dLength(direction));

    uint32_t initialIndex = records[2];
    S_004e0130_InsertIndex(
        frontier, nullptr, &initialIndex, static_cast<int>(initialRecord[8]));

    if (records[2] >= records[1])
        PathSearch_GrowRecords(records, nullptr, static_cast<int>(records[3]));

    uint32_t index = records[2];
    void* destination = reinterpret_cast<void*>(records[0] + 36u * index);
    records[2] = index + 1;
    std::memcpy(destination, initialRecord, 0x24);

    bool foundTarget = false;
    bool expansionFailed = false;
    for (;;)
    {
        uint32_t begin = frontier->field_0004;
        if (!begin ||
            (static_cast<int32_t>(frontier->field_0008 - begin) >> 3) == 0)
            break;

        int nextIndex = S_004e0130_PopNextIndex(frontier, nullptr);
        auto* record = reinterpret_cast<uint8_t*>(
            records[0] + 36u * static_cast<uint32_t>(nextIndex));
        currentRecord = static_cast<int>(reinterpret_cast<uintptr_t>(record));

        PathSearch_GetNodeData(
            reinterpret_cast<void*>(CSF_AT(uint32_t, record, 0x04)), nullptr);

        if (CSF_AT(float, record, 0x20) < closestDistance)
        {
            closestRecord = currentRecord;
            closestDistance = CSF_AT(float, record, 0x20);
        }

        foundTarget = CSF_AT(int, record, 0x04) == targetNode;
        if (foundTarget)
            break;

        if (!S_004dfbc0_ExpandRecord(
                self, nullptr, currentRecord, targetPosition, records, frontier))
        {
            expansionFailed = true;
            break;
        }
    }

    C_004de770_UnlockAndRelease(
        reinterpret_cast<void*>(self->field_0004), nullptr);

    if (!expansionFailed)
    {
        if (foundTarget)
            return currentRecord;
        if ((flags & 1) != 0)
            return closestRecord;
    }
    return 0;
}

HOOK(0x004E0440, S_004dfbc0_FindPathRecord, "auto");
