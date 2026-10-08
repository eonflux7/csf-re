#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __fastcall CMenuVideoOptions_HasVideoSettingsChanged(
    CMenuVideoOptions* self, void* /*edx*/)
{
    void* settings = reinterpret_cast<void*>(self->field_0024);
    if (!settings)
        return 0;

    const void* resolution =
        ((const void* (__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x20 / 4])(settings, nullptr);

    if (self->field_0028 != CSF_AT(std::uint32_t, resolution, 0))
        return 1;
    if (self->field_002c != CSF_AT(std::uint32_t, resolution, 4))
        return 1;

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint8_t mode =
        ((std::uint8_t (__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x38 / 4])(settings, nullptr);
    if (self->field_0039 != mode)
        return 1;

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t quality =
        ((std::uint32_t (__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x40 / 4])(settings, nullptr);
    if (self->field_003c != quality)
        return 1;

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t option =
        ((std::uint32_t (__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x58 / 4])(settings, nullptr);
    if (self->field_0048 != option)
        return 1;

    return 0;
}

HOOK(0x005495D0, CMenuVideoOptions_HasVideoSettingsChanged, "auto");
