#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00448a80_GrowStorage, 0x00448A80,
         (void* self, void* edx, int a2));

[[maybe_unused]] static const auto kStorageGrowthReference =
    CSF_VTBL(0x00448A80);

static uint32_t* __fastcall S_00448a80_UpsertEntry(
    S_00448a80* self, void* /*edx*/, uint32_t* incoming)
{
    const uint32_t count = self->field_0008;
    uint32_t* result = nullptr;
    bool found = false;

    if (count != 0)
    {
        auto* entry = reinterpret_cast<uint32_t*>(self->field_0000);
        for (uint32_t index = 0; index < count; ++index, entry += 6)
        {
            if (static_cast<int32_t>(entry[5]) >= 0 &&
                entry[1] == incoming[1])
            {
                result = entry;
                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        if (static_cast<int32_t>(incoming[5]) < 0)
            return reinterpret_cast<uint32_t*>(incoming[5]);

        if (count != 0)
        {
            auto* entry = reinterpret_cast<uint32_t*>(self->field_0000);
            for (uint32_t index = 0; index < count; ++index, entry += 6)
            {
                if (static_cast<int32_t>(entry[5]) < 0)
                {
                    result = entry;
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            if (count >= self->field_0004)
            {
                S_00448a80_GrowStorage(
                    self, nullptr, static_cast<int32_t>(self->field_000c));
            }

            const uint32_t index = self->field_0008;
            result = reinterpret_cast<uint32_t*>(
                self->field_0000 + 24 * index);
            self->field_0008 = index + 1;
        }
    }

    result[1] = incoming[1];
    result[2] = incoming[2];
    result[4] = incoming[4];
    result[5] = incoming[5];
    return result;
}

HOOK(0x00449440, S_00448a80_UpsertEntry, "auto");
