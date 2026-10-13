#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(uint8_t*, __fastcall, S_004f0cd0_RebuildRecords, 0x004F0F50,
         (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_004f0cd0_EnsureCategoryRecords, 0x004F1340,
         (void* self, void* edx, int a2));

static bool __fastcall S_004f0cd0_FindCategoryRecord(
    S_004f0cd0* self, void* /*edx*/, int category,
    const void* query, uint32_t* output)
{
    if (CSF_AT(int32_t, query, 0x08) < 0)
        return false;

    if (self->field_0040 && self->field_003c < g_fZero)
    {
        self->field_003c = 0.5f;
        S_004f0cd0_RebuildRecords(self, nullptr);
    }

    uint8_t* records = nullptr;
    if (S_004f0cd0_EnsureCategoryRecords(self, nullptr, category))
    {
        switch (category)
        {
        case 0:
            records = reinterpret_cast<uint8_t*>(self) + 0x2C;
            break;
        case 1:
            records = reinterpret_cast<uint8_t*>(self) + 0x1C;
            break;
        case 2:
            records = reinterpret_cast<uint8_t*>(self) + 0x0C;
            break;
        }
    }

    output[0] = -1;
    output[1] = -1;
    output[8] = 2139095039;
    output[9] = 0;

    if (records)
    {
        const uint32_t count = CSF_AT(uint32_t, records, 0x08);
        if (count)
        {
            uint8_t* entry = CSF_AT(uint8_t*, records, 0x00);
            const uint32_t queryId = CSF_AT(uint32_t, query, 0x04);
            uint32_t index = 0;

            while (CSF_AT(uint32_t, entry, 0x0C) != queryId)
            {
                ++index;
                entry += 0x28;
                if (index >= count)
                    return output[0] != uint32_t(-1);
            }

            output[0] = CSF_AT(uint32_t, entry, 0x00);
            output[1] = CSF_AT(uint32_t, entry, 0x04);
            output[3] = CSF_AT(uint32_t, entry, 0x0C);
            output[4] = CSF_AT(uint32_t, entry, 0x10);
            output[5] = CSF_AT(uint32_t, entry, 0x14);
            output[6] = CSF_AT(uint32_t, entry, 0x18);
            output[7] = CSF_AT(uint32_t, entry, 0x1C);
            output[8] = CSF_AT(uint32_t, entry, 0x20);
            output[9] = CSF_AT(uint32_t, entry, 0x24);
        }
    }

    return output[0] != uint32_t(-1);
}

HOOK(0x004F13B0, S_004f0cd0_FindCategoryRecord, "auto");
