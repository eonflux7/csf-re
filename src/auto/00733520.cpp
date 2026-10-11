#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_008670f0_ApplyPendingEntryStep(
    C_008670f0* self, void* /*edx*/)
{
    const uint32_t entryCount = self->field_0080;
    if (entryCount == 0)
        return;

    const uint32_t currentIndex = self->field_00f4;
    if (currentIndex == 0)
    {
        if (static_cast<int32_t>(self->field_00f8) < 0)
        {
            auto* entry = reinterpret_cast<uint8_t*>(self->field_0078);
            self->field_00f4 = 0;
            const int32_t origin = static_cast<int32_t>(self->field_0098);
            const float position = CSF_AT(float, entry, 0x4) - static_cast<float>(origin);
            self->field_00f8 = 0;
            self->field_00fc = 1;
            self->field_00ec = position;
            return;
        }

        if (static_cast<int32_t>(self->field_00f8) > 0 && self->field_00fc)
        {
            auto* entry = reinterpret_cast<uint8_t*>(self->field_0078);
            self->field_00f4 = 0;
            const int32_t origin = static_cast<int32_t>(self->field_0098);
            auto* entryObject =
                reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entry, 0x0));
            const float position =
                static_cast<float>(CSF_AT(int32_t, entryObject, 0x28)) *
                    self->field_00c0 +
                CSF_AT(float, entry, 0x4);
            self->field_00fc = 0;
            self->field_00ec = position - static_cast<float>(origin);
            self->field_00f8 = 0;
            return;
        }
    }

    if (currentIndex >= entryCount)
    {
        self->field_00f4 = entryCount - 1;
        self->field_00f8 = 0;
    }

    const int32_t nextIndex =
        static_cast<int32_t>(self->field_00f8 + self->field_00f4);
    if (nextIndex >= 0 && nextIndex < static_cast<int32_t>(entryCount))
    {
        auto* entries = reinterpret_cast<uint8_t*>(self->field_0078);
        const uint8_t useEntryStart = self->field_00fc;
        self->field_00f4 = static_cast<uint32_t>(nextIndex);
        auto* entry = entries + 28 * static_cast<uint32_t>(nextIndex);

        float extent;
        if (useEntryStart)
        {
            extent = g_fZero;
        }
        else
        {
            auto* entryObject =
                reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entry, 0x0));
            extent = static_cast<float>(CSF_AT(int32_t, entryObject, 0x28)) *
                     self->field_00c0;
        }

        const int32_t origin = static_cast<int32_t>(self->field_0098);
        self->field_00ec =
            extent + CSF_AT(float, entry, 0x4) - static_cast<float>(origin);
    }

    self->field_00f8 = 0;
}

HOOK(0x00733520, C_008670f0_ApplyPendingEntryStep, "auto");
