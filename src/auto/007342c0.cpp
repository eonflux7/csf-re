#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, _ceil, 0x007B1C40, (double X));

static uint32_t* __fastcall C_008670f0_GetWeightedItemTotal(
    C_008670f0* self, void* /*edx*/, uint32_t* output, int /*unused*/)
{
    uint32_t total = 0;
    int32_t lastValue = 0;

    if (self->field_0014)
    {
        float weightedTotal = 0.0f;
        const uint32_t itemCount = self->field_0034;

        for (uint32_t index = 0; index < itemCount; ++index)
        {
            const uint16_t* itemIds;
            if (self->field_0038 < 8)
                itemIds = reinterpret_cast<const uint16_t*>(&self->field_0024);
            else
                itemIds = reinterpret_cast<const uint16_t*>(self->field_0024);

            const uint32_t itemId = itemIds[index];
            void* provider = reinterpret_cast<void*>(self->field_0014);
            void* item =
                reinterpret_cast<void* (__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, provider, 0)[0x10 / 4])(
                    provider, nullptr, itemId);

            if (item)
            {
                provider = reinterpret_cast<void*>(self->field_0014);
                lastValue =
                    reinterpret_cast<int32_t (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, provider, 0)[0x18 / 4])(
                        provider, nullptr, 1);

                weightedTotal +=
                    static_cast<float>(CSF_AT(int32_t, item, 0x28)) *
                    self->field_00c0;
            }
        }

        total = static_cast<uint32_t>(
            static_cast<int64_t>(_ceil(static_cast<double>(weightedTotal))));
    }

    output[0] = total;
    output[1] = static_cast<uint32_t>(lastValue);
    return output;
}

HOOK(0x007342C0, C_008670f0_GetWeightedItemTotal, "auto");
