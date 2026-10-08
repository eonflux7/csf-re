#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, CGuiStatic_CopyStateToTarget, 0x00718230,
         (void* self, void* edx, void* target));
CSF_FUNC(void, __fastcall, MenuMessageBox_LayoutOptionControl, 0x005299A0,
         (void* self, void* edx, std::uint32_t index,
          std::uint32_t horizontalOffset, std::uint32_t finalComponent));
CSF_FUNC(int, __fastcall, MenuMessageBox_UpdateChildClipping, 0x00529910,
         (void* self, void* edx, int index));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_MessageBoxVerticalOffset, 0x00841300);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static void* MenuMessageBox_GetVirtualObject(void* object, std::uint32_t slot)
{
    return ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[slot])(object, nullptr);
}

static std::uint32_t MenuMessageBox_GetVirtualDimension(
    void* object, std::uint32_t slot)
{
    return ((std::uint32_t(__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[slot])(object, nullptr);
}

static void MenuMessageBox_SetVirtualPair(
    void* object, std::uint32_t slot, std::uint32_t* pair)
{
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, object, 0)[slot])(object, nullptr, pair);
}

static std::uint8_t __fastcall MenuMessageBox_UpdateLayout(
    void* self, void* /*edx*/)
{
    void* layout = MenuMessageBox_GetVirtualObject(
        CSF_AT(void*, self, 0x894), 0x1CC / 4);

    const std::uint8_t enabled = CSF_AT(std::uint8_t, self, 0x8B8);
    if (!enabled)
        return enabled;

    void* headerControl = CSF_AT(void*, self, 0x58);
    if (!headerControl || !layout ||
        !CSF_AT(void*, self, 0x5C) ||
        !CSF_AT(void*, self, 0x60) ||
        !CSF_AT(void*, self, 0x64) ||
        !CSF_AT(void*, self, 0x68))
    {
        return static_cast<std::uint8_t>(
            reinterpret_cast<std::uintptr_t>(headerControl));
    }

    void* context = GetGlobalInt();
    void* display = LookupObjectById(context, nullptr);
    void* resolution = MenuMessageBox_GetVirtualObject(display, 0x18 / 4);
    const double resolutionHeight =
        static_cast<double>(CSF_AT(std::int32_t, resolution, 4));

    const std::int32_t heightSign = static_cast<std::int32_t>(g_ScreenHeight);
    double screenHeight =
        static_cast<double>(static_cast<std::int32_t>(g_ScreenHeight));
    if (heightSign < 0)
        screenHeight += static_cast<double>(g_UnsignedIntegerBias);

    const float verticalScale =
        static_cast<float>(resolutionHeight / screenHeight);

    context = GetGlobalInt();
    display = LookupObjectById(context, nullptr);
    MenuMessageBox_GetVirtualObject(display, 0x18 / 4);

    void* header = MenuMessageBox_GetVirtualObject(
        CSF_AT(void*, self, 0x58), 0x110 / 4);
    const std::uint32_t headerHeight = header
        ? MenuMessageBox_GetVirtualDimension(header, 0xA0 / 4)
        : 0;

    std::uint32_t extent[2] = {0, 0};
    std::uint32_t position[2] = {0, 0};
    void* body = MenuMessageBox_GetVirtualObject(
        CSF_AT(void*, self, 0x5C), 0x110 / 4);

    std::uint32_t bodyWidth = 0;
    std::uint32_t bodyHeight = 0;
    if (body)
    {
        bodyWidth = MenuMessageBox_GetVirtualDimension(body, 0xA4 / 4);
        bodyHeight = MenuMessageBox_GetVirtualDimension(body, 0xA0 / 4);
    }

    void* layoutExtent =
        MenuMessageBox_GetVirtualObject(layout, 0x28 / 4);
    void* layoutPosition =
        MenuMessageBox_GetVirtualObject(layout, 0x20 / 4);

    const std::int32_t widthDifference = static_cast<std::int32_t>(
        CSF_AT(std::uint32_t, layoutExtent, 0) - bodyWidth);
    position[0] = static_cast<std::uint32_t>(widthDifference / 2)
                + CSF_AT(std::uint32_t, layoutPosition, 0);

    layoutPosition = MenuMessageBox_GetVirtualObject(layout, 0x20 / 4);
    const std::int64_t verticalOffset = static_cast<std::int64_t>(
        static_cast<double>(verticalScale) *
        static_cast<double>(g_MessageBoxVerticalOffset));
    position[1] = CSF_AT(std::uint32_t, layoutPosition, 4)
                - static_cast<std::uint32_t>(verticalOffset)
                + headerHeight;

    extent[0] = bodyWidth;
    extent[1] = bodyHeight;
    MenuMessageBox_SetVirtualPair(
        CSF_AT(void*, self, 0x5C), 0x98 / 4, position);
    MenuMessageBox_SetVirtualPair(
        CSF_AT(void*, self, 0x5C), 0xA0 / 4, extent);
    CGuiStatic_CopyStateToTarget(
        CSF_AT(void*, self, 0x5C), nullptr, body);

    void* firstOption = MenuMessageBox_GetVirtualObject(
        CSF_AT(void*, self, 0x60), 0x110 / 4);
    const std::uint32_t optionWidth = firstOption
        ? MenuMessageBox_GetVirtualDimension(firstOption, 0xA4 / 4)
        : 0;

    CSF_AT(std::uint32_t, self, 0x9A0) = optionWidth;
    MenuMessageBox_LayoutOptionControl(
        self, nullptr, 0, (0u - optionWidth) * 2u, 0x42C80000u);
    MenuMessageBox_LayoutOptionControl(
        self, nullptr, 1, 0u - optionWidth, 0x43480000u);
    MenuMessageBox_LayoutOptionControl(
        self, nullptr, 2, 0, 0x437F0000u);

    MenuMessageBox_UpdateChildClipping(self, nullptr, 0);
    MenuMessageBox_UpdateChildClipping(self, nullptr, 1);
    return static_cast<std::uint8_t>(
        MenuMessageBox_UpdateChildClipping(self, nullptr, 2));
}

HOOK(0x0052A830, MenuMessageBox_UpdateLayout, "auto");
