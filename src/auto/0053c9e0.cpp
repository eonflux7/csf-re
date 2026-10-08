#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __fastcall CMenuSoundOptions_HandleVolumeSliderEvent(
    CMenuSoundOptions* self, void* /*edx*/, void* event)
{
    if (!self->field_002c)
        return 0;

    void* slider = reinterpret_cast<void*>(self->field_0040);
    if (slider &&
        reinterpret_cast<std::uint32_t>(slider) ==
            CSF_AT(std::uint32_t, event, 0x04))
    {
        const float position = static_cast<float>(
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, slider, 0)[0x274 / 4])(slider, nullptr));

        const int steps =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, slider, 0)[0x264 / 4])(slider, nullptr);

        const std::int32_t denominator =
            static_cast<std::int32_t>(static_cast<std::uint32_t>(steps) - 1u);

        void* settings = reinterpret_cast<void*>(self->field_002c);
        void** settingsVtable = CSF_AT(void**, settings, 0);
        const float volume = static_cast<float>(
            static_cast<double>(position) / static_cast<double>(denominator));

        return ((int(__fastcall*)(void*, void*, float))
            settingsVtable[0x1C / 4])(settings, nullptr, volume);
    }

    slider = reinterpret_cast<void*>(self->field_0044);
    if (slider &&
        reinterpret_cast<std::uint32_t>(slider) ==
            CSF_AT(std::uint32_t, event, 0x04))
    {
        const float position = static_cast<float>(
            ((double(__fastcall*)(void*, void*))
                CSF_AT(void**, slider, 0)[0x274 / 4])(slider, nullptr));

        const int steps =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, slider, 0)[0x264 / 4])(slider, nullptr);

        const std::int32_t denominator =
            static_cast<std::int32_t>(static_cast<std::uint32_t>(steps) - 1u);

        void* settings = reinterpret_cast<void*>(self->field_002c);
        void** settingsVtable = CSF_AT(void**, settings, 0);
        const float volume = static_cast<float>(
            static_cast<double>(position) / static_cast<double>(denominator));

        return ((int(__fastcall*)(void*, void*, float))
            settingsVtable[0x24 / 4])(settings, nullptr, volume);
    }

    return ((int(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x54 / 4])(self, nullptr, event);
}

HOOK(0x0053C9E0, CMenuSoundOptions_HandleVolumeSliderEvent, "auto");
