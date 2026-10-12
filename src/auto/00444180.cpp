#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, GrowRecordStorage, 0x00443E00,
         (void* self, void* edx, int a2));

static int __fastcall S_00444180_InsertRecord(
    S_00444180* self, void* /*edx*/, int recordAddress)
{
    const uint32_t originalCount = self->field_0014;
    int freeIndex = -1;

    if (originalCount != 0)
    {
        const auto* record = reinterpret_cast<const uint8_t*>(self->field_000c);
        uint32_t index = 0;
        while (*record != 0)
        {
            ++index;
            record += 40;
            if (index >= originalCount)
                break;
        }
        if (index < originalCount)
            freeIndex = static_cast<int>(index);
    }

    uint8_t* destination;
    int result;
    if (freeIndex == -1)
    {
        if (self->field_0014 >= CSF_AT(uint32_t, self, 0x10))
        {
            GrowRecordStorage(
                &self->field_000c, nullptr, CSF_AT(int, self, 0x18));
        }

        const uint32_t appendIndex = self->field_0014;
        destination = reinterpret_cast<uint8_t*>(self->field_000c)
                    + 40u * appendIndex;
        self->field_0014 = appendIndex + 1;
        result = static_cast<int>(originalCount);
    }
    else
    {
        destination = reinterpret_cast<uint8_t*>(self->field_000c)
                    + 40u * static_cast<uint32_t>(freeIndex);
        result = freeIndex;
    }

    const auto* source = reinterpret_cast<const uint8_t*>(recordAddress);
    CSF_AT(uint8_t, destination, 0x00) = CSF_AT(uint8_t, source, 0x00);
    CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, source, 0x08);
    CSF_AT(uint32_t, destination, 0x0C) = CSF_AT(uint32_t, source, 0x0C);
    CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, source, 0x10);
    CSF_AT(uint32_t, destination, 0x14) = CSF_AT(uint32_t, source, 0x14);
    CSF_AT(uint32_t, destination, 0x18) = CSF_AT(uint32_t, source, 0x18);
    CSF_AT(uint32_t, destination, 0x1C) = CSF_AT(uint32_t, source, 0x1C);
    CSF_AT(uint32_t, destination, 0x20) = CSF_AT(uint32_t, source, 0x20);
    CSF_AT(uint32_t, destination, 0x24) = CSF_AT(uint32_t, source, 0x24);
    return result;
}

HOOK(0x00444180, S_00444180_InsertRecord, "auto");
