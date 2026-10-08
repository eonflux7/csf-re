#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MessageBoxCollapsedScale, 0x0082DFCC);

static std::uint32_t* __fastcall MenuMessageBox_CollapseLayout(
    void* self, void* /*edx*/)
{
    void* window = CSF_AT(void*, self, 0x894);
    CSF_AT(std::uint32_t, self, 0x974) = 0;

    auto* result =
        reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, window, 0)[0x1CC / 4])(window, nullptr);

    if (!result)
        return result;

    void* layout = result;
    const double originalWidth = CSF_AT(std::int32_t, self, 0x990);
    const std::int32_t collapsedWidth = static_cast<std::int32_t>(
        static_cast<std::int64_t>(
            originalWidth * static_cast<double>(g_MessageBoxCollapsedScale)));

    const double originalHeight = CSF_AT(std::int32_t, self, 0x994);
    const float roundedHeight = static_cast<float>(originalHeight);
    CSF_AT(std::int32_t, self, 0x980) = collapsedWidth;

    const std::int32_t collapsedHeight = static_cast<std::int32_t>(
        static_cast<std::int64_t>(
            originalHeight * static_cast<double>(g_MessageBoxCollapsedScale)));

    const double originalX = CSF_AT(std::int32_t, self, 0x988);
    const double halfWidth = originalWidth * static_cast<double>(g_HalfDurationScale);
    CSF_AT(std::int32_t, self, 0x984) = collapsedHeight;

    const std::uint32_t collapsedX = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            (originalX + halfWidth) -
            static_cast<double>(collapsedWidth) *
                static_cast<double>(g_HalfDurationScale)));

    const double originalY = CSF_AT(std::int32_t, self, 0x98C);
    CSF_AT(std::uint32_t, self, 0x978) = collapsedX;

    CSF_AT(std::uint32_t, self, 0x97C) = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            (originalY + static_cast<double>(roundedHeight) *
                             static_cast<double>(g_HalfDurationScale)) -
            static_cast<double>(collapsedHeight) *
                static_cast<double>(g_HalfDurationScale)));

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, layout, 0)[0x1C / 4])(
            layout, nullptr, static_cast<std::uint8_t*>(self) + 0x978);

    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, layout, 0)[0x24 / 4])(
            layout, nullptr, static_cast<std::uint8_t*>(self) + 0x980);

    const std::uint32_t x = CSF_AT(std::uint32_t, self, 0x978);
    const std::uint32_t y = CSF_AT(std::uint32_t, self, 0x97C);
    const std::uint32_t width = CSF_AT(std::uint32_t, self, 0x980);
    const std::uint32_t height = CSF_AT(std::uint32_t, self, 0x984);
    std::uint32_t rectangle[4] = {x, y, x + width, y + height};

    window = CSF_AT(void*, self, 0x894);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*, int)>(
        CSF_AT(void**, window, 0)[0x208 / 4])(
            window, nullptr, rectangle, 0);

    window = CSF_AT(void*, self, 0x894);
    result = reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, window, 0)[0x1DC / 4])(window, nullptr);

    if (result)
    {
        auto clearRectangle =
            reinterpret_cast<std::uint32_t* (__fastcall*)(
                void*, void*, std::uint32_t*)>(
                    CSF_AT(void**, result, 0)[0x2C / 4]);
        std::uint32_t emptyRectangle[4] = {0, 0, 0, 0};
        result = clearRectangle(result, nullptr, emptyRectangle);
    }

    CSF_AT(std::uint32_t, self, 0x28) = 0;
    return result;
}

HOOK(0x00528AE0, MenuMessageBox_CollapseLayout, "auto");
