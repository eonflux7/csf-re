#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __fastcall CMenuVideoOptions_ApplyVideoSettings(
    CMenuVideoOptions* self, void* /*edx*/, const void* values)
{
    void* settings = reinterpret_cast<void*>(self->field_0024);
    if (!settings)
        return 0;

    const std::uint32_t height = CSF_AT(std::uint32_t, values, 0x04);
    const std::uint32_t width = CSF_AT(std::uint32_t, values, 0x00);
    ((void (__fastcall*)(void*, void*, std::uint32_t, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x24 / 4])(
            settings, nullptr, width, height);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value08 = CSF_AT(std::uint32_t, values, 0x08);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x1C / 4])(
            settings, nullptr, value08);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value0C = CSF_AT(std::uint32_t, values, 0x0C);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x2C / 4])(
            settings, nullptr, value0C);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t flag10 = CSF_AT(std::uint8_t, values, 0x10);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x34 / 4])(
            settings, nullptr, flag10);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t flag11 = CSF_AT(std::uint8_t, values, 0x11);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x3C / 4])(
            settings, nullptr, flag11);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value14 = CSF_AT(std::uint32_t, values, 0x14);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x44 / 4])(
            settings, nullptr, value14);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value18 = CSF_AT(std::uint32_t, values, 0x18);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x4C / 4])(
            settings, nullptr, value18);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value1C = CSF_AT(std::uint32_t, values, 0x1C);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x54 / 4])(
            settings, nullptr, value1C);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value20 = CSF_AT(std::uint32_t, values, 0x20);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x5C / 4])(
            settings, nullptr, value20);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value24 = CSF_AT(std::uint32_t, values, 0x24);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x64 / 4])(
            settings, nullptr, value24);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value28 = CSF_AT(std::uint32_t, values, 0x28);
    ((void (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x6C / 4])(
            settings, nullptr, value28);

    settings = reinterpret_cast<void*>(self->field_0024);
    const std::uint32_t value2C = CSF_AT(std::uint32_t, values, 0x2C);
    return ((int (__fastcall*)(void*, void*, std::uint32_t))
        CSF_AT(void**, settings, 0)[0x74 / 4])(
            settings, nullptr, value2C);
}

HOOK(0x005496A0, CMenuVideoOptions_ApplyVideoSettings, "auto");
