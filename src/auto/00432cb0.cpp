#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_CreateWidget, 0x0042D750, (void* a1, int a2));
CSF_FUNC(void*, __fastcall, Hud_GrowWidgetArray, 0x0042FC20, (void* self, void* edx, int a2));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(const char*, g_pszKey_ID_hud, 0x008995A8);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static const auto kMenuExtrasVtable = CSF_VTBL(0x00692B80);

// Native unsigned conversion replaces the compiler's x87 bias correction.
static std::uint32_t Hud_PercentToPixels(std::uint32_t product)
{
    return static_cast<std::uint32_t>(
        static_cast<double>(product) * g_ProgressScale);
}

static void __fastcall Hud_InitializeCrosshairWidgets(
    void* self, void* /*edx*/, void* configuration)
{
    CSF_AT(std::uint32_t, self, 0x148) = 0;
    if (!configuration)
        return;

    const std::uint32_t childCount =
        MenuExtras_GetChildCount(configuration, nullptr);
    const std::uint32_t requiredCount = childCount > 2 ? childCount : 2;
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x144);
    auto* widgetArray = static_cast<std::uint8_t*>(self) + 0x140;

    if (requiredCount > capacity)
        Hud_GrowWidgetArray(
            widgetArray, nullptr, static_cast<int>(requiredCount - capacity));

    CSF_AT(std::uint32_t, self, 0x148) = requiredCount;

    std::uint32_t entryOffset = 0;
    for (std::uint32_t index = 0; index < childCount; ++index, entryOffset += 12)
    {
        CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x140), entryOffset) = 0;

        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(static_cast<unsigned char*>(configuration), nullptr, index));
        void* widget = reinterpret_cast<void*>(Hud_CreateWidget(component, 4));

        char identifier[260];
        MenuExtras_ReadText(
            component, nullptr, const_cast<char*>(g_pszKey_ID_hud), identifier, 256);

        int slot = __stricmp(identifier, CSF_STR(0x008308F4, "NORMAL"));
        if (slot != 0)
        {
            if (__stricmp(identifier, CSF_STR(0x008308E8, "DISPERSION")) != 0)
                continue;
            slot = 1;
        }

        const std::uint32_t slotOffset = 12 * slot;
        CSF_AT(void*, CSF_AT(void*, self, 0x140), slotOffset) = widget;
        if (!CSF_AT(void*, CSF_AT(void*, self, 0x140), slotOffset))
            continue;

        const auto* position =
            reinterpret_cast<const std::uint32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, widget, 0)[0x2C / 4])(widget, nullptr);
        const std::uint32_t positionX = position[0];
        const std::uint32_t positionY = position[1];

        const auto* size =
            reinterpret_cast<const std::uint32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, widget, 0)[0x3C / 4])(widget, nullptr);
        const std::uint32_t sizeY = size[1];
        const std::uint32_t screenWidth = g_ScreenWidth;
        const std::uint32_t pixelWidth =
            Hud_PercentToPixels(size[0] * screenWidth);
        const std::uint32_t pixelHeight =
            Hud_PercentToPixels(sizeY * g_ScreenHeight);

        widget = CSF_AT(void*, CSF_AT(void*, self, 0x140), slotOffset);
        std::uint32_t pixelPosition[2];
        pixelPosition[0] = Hud_PercentToPixels(positionX * screenWidth);
        pixelPosition[1] = Hud_PercentToPixels(positionY * g_ScreenHeight);

        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, widget, 0)[0x28 / 4])(widget, nullptr, pixelPosition);

        widget = CSF_AT(void*, CSF_AT(void*, self, 0x140), slotOffset);
        std::uint32_t pixelSize[2] = {pixelWidth, pixelHeight};
        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, widget, 0)[0x38 / 4])(widget, nullptr, pixelSize);

        auto* entry = static_cast<std::uint8_t*>(CSF_AT(void*, self, 0x140))
            + slotOffset;
        widget = CSF_AT(void*, entry, 0);
        const auto* resultingSize =
            reinterpret_cast<const std::uint32_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, widget, 0)[0x40 / 4])(widget, nullptr);
        CSF_AT(std::uint32_t, entry, 4) = resultingSize[0];
        CSF_AT(std::uint32_t, entry, 8) = resultingSize[1];
    }
}

HOOK(0x00432CB0, Hud_InitializeCrosshairWidgets, "auto");
