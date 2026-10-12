#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __fastcall, TypeRegistry_FindMeasurementEntry, 0x0054B3C0,
         (void* self, void* edx, void** a2, void** a3));
CSF_FUNC(int, __fastcall, TypeRegistry_InsertMeasurementEntry, 0x0054B900,
         (void* self, void* edx, int a2, void** a3));

static int* __fastcall C_008442a0_AccumulateLastEntryMeasurement(
    C_008442a0* self, void* /*edx*/)
{
    const uint32_t begin = self->field_008c;
    const int32_t count = begin
        ? static_cast<int32_t>(self->field_0090 - begin) >> 3
        : 0;

    auto* entries = reinterpret_cast<uint32_t*>(self->field_008c);

    alignas(8) uint8_t keyBytes[0x4];
    void*& key = *reinterpret_cast<void**>(keyBytes);
    key = reinterpret_cast<void*>(entries[2 * count - 1]);

    alignas(8) uint8_t entryBytes[0x4];
    void*& entry = *reinterpret_cast<void**>(entryBytes);
    entry = entries;

    const int32_t measurementCount = entries
        ? static_cast<int32_t>(
              self->field_0090 - reinterpret_cast<uint32_t>(entries)) >> 3
        : 0;

    void* source = reinterpret_cast<void*>(self->field_00a4);
    const int measurement =
        (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, source, 0)[0x90 / 4]))(source, nullptr);
    const uint32_t elapsed = static_cast<uint32_t>(measurement)
        - static_cast<uint32_t*>(entry)[2 * measurementCount - 2];

    void* measurementEntries = reinterpret_cast<uint8_t*>(self) + 0x98;
    TypeRegistry_FindMeasurementEntry(
        measurementEntries, nullptr, &entry, &key);

    if (entry == reinterpret_cast<void*>(self->field_009c))
    {
        alignas(8) uint8_t recordBytes[0xC];
        auto** record = reinterpret_cast<void**>(recordBytes);
        record[0] = key;
        record[1] = reinterpret_cast<void*>(1);
        record[2] = reinterpret_cast<void*>(elapsed);

        alignas(8) uint8_t insertionResultBytes[0x4];
        return reinterpret_cast<int*>(TypeRegistry_InsertMeasurementEntry(
            measurementEntries, nullptr,
            reinterpret_cast<int>(insertionResultBytes), record));
    }

    CSF_AT(uint32_t, entry, 0x14) += elapsed;
    ++CSF_AT(uint32_t, entry, 0x10);
    return reinterpret_cast<int*>(entry);
}

HOOK(0x0054BD20, C_008442a0_AccumulateLastEntryMeasurement, "auto");
