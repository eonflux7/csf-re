#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(char, __fastcall, IsTrackedHandleActive, 0x0045BDA0, (S_0045bda0* self, void*));

[[maybe_unused]] static const auto kTrackedHandleVtable = CSF_VTBL(0x0045BDA0);
[[maybe_unused]] static const auto kButtonArrayVtable = CSF_VTBL(0x0041E5F0);

static unsigned int __fastcall CMissionScript_AppendActiveHandlesForClass(
    C_008314f8* self, void* /*edx*/, S_0041e5f0* output, int classIndex)
{
    const uint32_t offset = static_cast<uint32_t>(classIndex) * 16;
    auto* primaryList =
        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x68)) + offset;
    const uint32_t primaryCount = CSF_AT(uint32_t, primaryList, 8);

    for (uint32_t index = 0; index < primaryCount; ++index)
    {
        const uint32_t handle =
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, primaryList, 0))[index];

        if (handle &&
            IsTrackedHandleActive(reinterpret_cast<S_0045bda0*>(handle), nullptr))
        {
            if (static_cast<uint32_t>(output->field_0008) >=
                static_cast<uint32_t>(output->field_0004))
            {
                GrowButtonArray(output, nullptr,
                                static_cast<int>(output->field_000c));
            }

            const uint32_t count = output->field_0008;
            auto* destination =
                reinterpret_cast<uint32_t*>(output->field_0000) + count;
            output->field_0008 = count + 1;
            *destination = handle;
        }
    }

    auto* secondaryList =
        reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x78)) + offset;
    const uint32_t secondaryCount = CSF_AT(uint32_t, secondaryList, 8);
    uint32_t index = 0;

    for (; index < secondaryCount; ++index)
    {
        const uint32_t handle =
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, secondaryList, 0))[index];

        if (handle &&
            IsTrackedHandleActive(reinterpret_cast<S_0045bda0*>(handle), nullptr))
        {
            if (static_cast<uint32_t>(output->field_0008) >=
                static_cast<uint32_t>(output->field_0004))
            {
                GrowButtonArray(output, nullptr,
                                static_cast<int>(output->field_000c));
            }

            const uint32_t count = output->field_0008;
            auto* destination =
                reinterpret_cast<uint32_t*>(output->field_0000) + count;
            output->field_0008 = count + 1;
            *destination = handle;
        }
    }

    return index;
}

HOOK(0x00468D40, CMissionScript_AppendActiveHandlesForClass, "auto");
