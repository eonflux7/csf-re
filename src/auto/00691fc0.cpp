#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, CoreMemory_GrowRecordArray, 0x00691730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoreMemory_InsertRecord, 0x006918E0, (void* self, void* edx, int a2, uint32_t* a3));

static uint32_t* __fastcall S_00691860_AppendOrInsertRecord(
    S_00691860* self, void* /*edx*/, uint32_t* record, int insertionAddress)
{
    // The packet also lists this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    const uint32_t chunkCount = self->field_000c;
    auto* chunks = reinterpret_cast<S_0041e5f0*>(
        reinterpret_cast<uint8_t*>(self) + 4);
    auto* chunkPointers = reinterpret_cast<uint32_t*>(self->field_0004);
    auto* chunk = reinterpret_cast<uint32_t*>(chunkPointers[chunkCount - 1]);
    const uint32_t recordCount = chunk[2];

    if (recordCount == 1000)
    {
        SetAllocationMode(1);
        auto* newChunk = static_cast<uint32_t*>(Mem_Alloc(0x10u));
        if (newChunk)
        {
            newChunk[0] = 0;
            newChunk[1] = 0;
            newChunk[2] = 0;
            newChunk[3] = 10;
        }
        SetAllocationMode(0);

        const uint32_t capacity = newChunk[1];
        if (capacity < 0x3E8)
            CoreMemory_GrowRecordArray(
                newChunk, nullptr, static_cast<int>(1000 - capacity));

        if (newChunk[2] >= newChunk[1])
            CoreMemory_GrowRecordArray(
                newChunk, nullptr, static_cast<int>(newChunk[3]));

        const uint32_t index = newChunk[2];
        auto* destination = reinterpret_cast<uint32_t*>(newChunk[0] + 16 * index);
        newChunk[2] = index + 1;
        destination[0] = record[0];
        destination[1] = record[1];
        destination[2] = record[2];
        destination[3] = record[3];

        if (chunks->field_0008 >= chunks->field_0004)
            S_0041e5f0_GrowArrayCapacity(
                chunks, nullptr, static_cast<int>(chunks->field_000c));

        const uint32_t chunkIndex = chunks->field_0008;
        auto* slot = reinterpret_cast<uint32_t*>(chunks->field_0000 + 4 * chunkIndex);
        const uint32_t newCount = chunkIndex + 1;
        chunks->field_0008 = newCount;
        *slot = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(newChunk));
        return reinterpret_cast<uint32_t*>(newCount);
    }

    if (insertionAddress)
    {
        const int32_t byteOffset = static_cast<int32_t>(
            static_cast<uint32_t>(insertionAddress) - chunk[0]);
        return reinterpret_cast<uint32_t*>(
            CoreMemory_InsertRecord(chunk, nullptr, byteOffset >> 4, record));
    }

    if (recordCount >= chunk[1])
        CoreMemory_GrowRecordArray(chunk, nullptr, static_cast<int>(chunk[3]));

    const uint32_t index = chunk[2];
    auto* destination = reinterpret_cast<uint32_t*>(chunk[0] + 16 * index);
    chunk[2] = index + 1;
    destination[0] = record[0];
    destination[1] = record[1];
    destination[2] = record[2];
    destination[3] = record[3];
    return destination;
}

HOOK(0x00691FC0, S_00691860_AppendOrInsertRecord, "auto");
