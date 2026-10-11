#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_WheelProcessingDistanceSquared, 0x0082E260);

CSF_FUNC(void, __fastcall, C_0082e1e8_ProcessWheelEntry, 0x00418660,
         (void* self, void* edx, int a2));

static int __fastcall C_0082e1e8_ProcessWheelEntries(
    C_0082e1e8* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* state =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    const bool processAlternately =
        CSF_AT(uint32_t, state, 0x78) == 0xBF800000u ||
        CSF_AT(float, state, 0x78) > g_WheelProcessingDistanceSquared;

    const bool alternateEnabled = self->field_0084 != 0;
    const int count = static_cast<int>(self->field_003c);

    if ((alternateEnabled || processAlternately) && count >= 4)
    {
        uint32_t offset = 0;
        for (int index = 0; index < count; ++index, offset += 32)
        {
            const uint32_t primary = self->field_0034 + offset;
            const uint32_t secondary = self->field_0044 + offset;

            switch (self->field_0088)
            {
            case 0u:
                if ((index & 1) == 0)
                    C_0082e1e8_ProcessWheelEntry(
                        self, nullptr, static_cast<int>(primary));
                break;
            case 1u:
                if ((index & 1) != 0)
                    C_0082e1e8_ProcessWheelEntry(
                        self, nullptr, static_cast<int>(secondary));
                break;
            case 2u:
                if ((index & 1) != 0)
                    C_0082e1e8_ProcessWheelEntry(
                        self, nullptr, static_cast<int>(primary));
                break;
            case 3u:
                if ((index & 1) == 0)
                    C_0082e1e8_ProcessWheelEntry(
                        self, nullptr, static_cast<int>(secondary));
                break;
            default:
                break;
            }
        }

        self->field_0088 =
            static_cast<int>(self->field_0088 + 1u) % 4;
        return count;
    }

    if (count > 0)
    {
        uint32_t offset = 0;
        int remaining = count;
        do
        {
            const uint32_t primary = self->field_0034 + offset;
            const uint32_t secondary = self->field_0044 + offset;
            C_0082e1e8_ProcessWheelEntry(
                self, nullptr, static_cast<int>(primary));
            C_0082e1e8_ProcessWheelEntry(
                self, nullptr, static_cast<int>(secondary));
            offset += 32;
            --remaining;
        } while (remaining != 0);
        return remaining;
    }

    return count;
}

HOOK(0x00419810, C_0082e1e8_ProcessWheelEntries, "auto");
