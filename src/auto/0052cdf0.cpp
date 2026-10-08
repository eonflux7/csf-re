#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall MenuProgressInput_ResetTextInputLayout(
    void* self, void* /*edx*/)
{
    auto* bytes = static_cast<std::uint8_t*>(self);

    void* geometry = CSF_AT(void*, self, 0x34);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, geometry, 0)[0x24 / 4])(
            geometry, nullptr, bytes + 0x148);

    geometry = CSF_AT(void*, self, 0x34);
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, geometry, 0)[0x1C / 4])(
            geometry, nullptr, bytes + 0x140);

    const std::uint32_t x = CSF_AT(std::uint32_t, self, 0x140);
    const std::uint32_t y = CSF_AT(std::uint32_t, self, 0x144);
    const std::uint32_t width = CSF_AT(std::uint32_t, self, 0x148);
    const std::uint32_t height = CSF_AT(std::uint32_t, self, 0x14C);
    std::uint32_t rectangle[4] = {x, y, x + width, y + height};

    void* layout = CSF_AT(void*, self, 0x1C);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*, int)>(
        CSF_AT(void**, layout, 0)[0x208 / 4])(
            layout, nullptr, rectangle, 0);

    void* appearance = CSF_AT(void*, self, 0x38);
    std::uint32_t appearanceValues[4] = {0, 0, 0, 0x42FE0000u};
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, appearance, 0)[0x2C / 4])(
            appearance, nullptr, appearanceValues);

    CSF_AT(std::uint32_t, self, 0x20) = 0xFFFFFFFFu;

    int result = 0;
    for (std::uint32_t offset = 0x24; offset != 0x34; offset += 4)
    {
        void* child = CSF_AT(void*, self, offset);
        result = reinterpret_cast<int>(child);
        if (child)
        {
            result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, child, 0)[0xA8 / 4])(child, nullptr, 0);
        }
    }
    return result;
}

HOOK(0x0052CDF0, MenuProgressInput_ResetTextInputLayout, "auto");
