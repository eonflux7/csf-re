#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuMessageBox_GetDisplayInterface, 0x00652160,
         (void* self, void* edx));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_MessageBoxVerticalOffset, 0x00841300);
CSF_GLOBAL(volatile std::uint32_t, g_ScreenHeight, 0x00899BA0);

static char __fastcall MenuMessageBox_UpdateLayerLayout(
    void* self, void* /*edx*/)
{
    using GetObject = void* (__fastcall*)(void*, void*);
    using GetInteger = std::uint32_t (__fastcall*)(void*, void*);
    using SetVector = void (__fastcall*)(void*, void*, std::int32_t*);
    using SetLayerState = char (__fastcall*)(void*, void*, int);

    void* owner = CSF_AT(void*, self, 0x894);
    void* anchor = reinterpret_cast<GetObject>(
        CSF_AT(void**, owner, 0)[0x1CC / 4])(owner, nullptr);

    const char layoutEnabled = CSF_AT(char, self, 0x8B9);
    if (!layoutEnabled || !anchor)
        return layoutEnabled;

    if (!CSF_AT(void*, self, 0x6C))
        return 0;
    if (!CSF_AT(void*, self, 0x70))
        return 0;
    if (!CSF_AT(void*, self, 0x74))
        return 0;

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* display = MenuMessageBox_GetDisplayInterface(globalObject, nullptr);
    void* displaySize = reinterpret_cast<GetObject>(
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

    const double displayHeight =
        static_cast<double>(CSF_AT(std::int32_t, displaySize, 4));
    const std::uint32_t screenHeightBits = g_ScreenHeight;
    double screenHeight =
        static_cast<double>(static_cast<std::int32_t>(g_ScreenHeight));
    if (static_cast<std::int32_t>(screenHeightBits) < 0)
        screenHeight += static_cast<double>(g_UnsignedIntegerBias);
    const float heightScale = static_cast<float>(displayHeight / screenHeight);

    globalObject = reinterpret_cast<void*>(GetGlobalInt());
    display = MenuMessageBox_GetDisplayInterface(globalObject, nullptr);
    reinterpret_cast<GetObject>(
        CSF_AT(void**, display, 0)[0x18 / 4])(display, nullptr);

    void* relatedOwner = CSF_AT(void*, self, 0x58);
    void* relatedObject = reinterpret_cast<GetObject>(
        CSF_AT(void**, relatedOwner, 0)[0x110 / 4])(relatedOwner, nullptr);
    const std::uint32_t verticalAdjustment = relatedObject
        ? reinterpret_cast<GetInteger>(
              CSF_AT(void**, relatedObject, 0)[0xA0 / 4])(
              relatedObject, nullptr)
        : 0;

    std::int32_t size[2] = {0, 0};
    std::int32_t position[2] = {0, 0};

    void* layer = CSF_AT(void*, self, 0x6C);
    void* layerSize = reinterpret_cast<GetObject>(
        CSF_AT(void**, layer, 0)[0xA4 / 4])(layer, nullptr);

    layer = CSF_AT(void*, self, 0x6C);
    GetObject getLayerSize = reinterpret_cast<GetObject>(
        CSF_AT(void**, layer, 0)[0xA4 / 4]);
    const std::uint32_t layerWidth =
        CSF_AT(std::uint32_t, layerSize, 0);
    layerSize = getLayerSize(layer, nullptr);
    const std::uint32_t layerHeight =
        CSF_AT(std::uint32_t, layerSize, 4);

    void* anchorSize = reinterpret_cast<GetObject>(
        CSF_AT(void**, anchor, 0)[0x28 / 4])(anchor, nullptr);
    void* anchorPosition = reinterpret_cast<GetObject>(
        CSF_AT(void**, anchor, 0)[0x20 / 4])(anchor, nullptr);

    const std::uint32_t widthDifference =
        CSF_AT(std::uint32_t, anchorSize, 0) - layerWidth;
    const std::int32_t horizontalAdjustment =
        static_cast<std::int32_t>(widthDifference) / 2;
    position[0] = static_cast<std::int32_t>(
        CSF_AT(std::uint32_t, anchorPosition, 0) +
        static_cast<std::uint32_t>(horizontalAdjustment));

    anchorPosition = reinterpret_cast<GetObject>(
        CSF_AT(void**, anchor, 0)[0x20 / 4])(anchor, nullptr);
    const std::int64_t scaledVerticalOffset = static_cast<std::int64_t>(
        static_cast<double>(heightScale) *
        static_cast<double>(g_MessageBoxVerticalOffset));

    const std::uint32_t positionY =
        CSF_AT(std::uint32_t, anchorPosition, 4) -
        static_cast<std::uint32_t>(scaledVerticalOffset) +
        verticalAdjustment;
    size[0] = static_cast<std::int32_t>(layerWidth);
    position[1] = static_cast<std::int32_t>(positionY);
    size[1] = static_cast<std::int32_t>(layerHeight);

    auto* layerField = reinterpret_cast<std::uint8_t*>(self) + 0x6C;
    for (int remaining = 3; remaining != 0; --remaining, layerField += 4)
    {
        layer = CSF_AT(void*, layerField, 0);
        reinterpret_cast<SetVector>(
            CSF_AT(void**, layer, 0)[0x98 / 4])(layer, nullptr, position);

        layer = CSF_AT(void*, layerField, 0);
        reinterpret_cast<SetVector>(
            CSF_AT(void**, layer, 0)[0xA0 / 4])(layer, nullptr, size);
    }

    layer = CSF_AT(void*, self, 0x6C);
    reinterpret_cast<SetLayerState>(
        CSF_AT(void**, layer, 0)[0xA8 / 4])(layer, nullptr, 0);

    layer = CSF_AT(void*, self, 0x70);
    reinterpret_cast<SetLayerState>(
        CSF_AT(void**, layer, 0)[0xA8 / 4])(layer, nullptr, 1);

    layer = CSF_AT(void*, self, 0x74);
    return reinterpret_cast<SetLayerState>(
        CSF_AT(void**, layer, 0)[0xA8 / 4])(layer, nullptr, 1);
}

HOOK(0x00529770, MenuMessageBox_UpdateLayerLayout, "auto");
