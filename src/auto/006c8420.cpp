#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxSkin_ShouldSkipProviderAttachment, 0x0068ECA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(char, __fastcall, GfxSkin_UpdateWidgetAttachment, 0x006C4740, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxSkin_GrowWidgetList, 0x006C7DD0, (void* self, void* edx, int a2));

static int __fastcall C_0085e7b8_AttachWidget(
    C_0085e7b8* self, void* /*edx*/, C_0085e7b8* widget)
{
    auto* provider = static_cast<C_0085e7b8*>(
        GetCachedObjectProvider(widget, nullptr));

    bool attachedProvider = false;
    if (provider &&
        !GfxSkin_ShouldSkipProviderAttachment(provider, nullptr))
    {
        C_0085e7b8_AttachWidget(self, nullptr, provider);
        attachedProvider = true;
    }

    auto* widgetList = reinterpret_cast<C_0085e7b8*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x14);

    const std::uint32_t previousCount = widgetList->field_0008;
    int previousWidget = 0;
    if (previousCount)
    {
        previousWidget = CSF_AT(
            int, reinterpret_cast<std::uint8_t*>(widgetList->vtbl),
            12 * previousCount - 12);
    }

    C_008670f0_SetField0C(
        reinterpret_cast<C_008670f0*>(widget), nullptr, previousWidget);
    GfxSkin_UpdateWidgetAttachment(widget, nullptr);

    if (widgetList->field_0008 >= widgetList->field_0004)
    {
        GfxSkin_GrowWidgetList(
            widgetList, nullptr, static_cast<int>(widgetList->field_000c));
    }

    const std::uint32_t count = widgetList->field_0008;
    auto* entry = reinterpret_cast<std::uint8_t*>(widgetList->vtbl)
        + 12 * count;
    const std::uint32_t newCount = count + 1;

    widgetList->field_0008 = newCount;
    CSF_AT(C_0085e7b8*, entry, 0x0) = widget;
    CSF_AT(C_0085e7b8*, entry, 0x4) = provider;
    CSF_AT(std::uint32_t, entry, 0x8) = attachedProvider ? 1u : 0u;

    return static_cast<int>(newCount);
}

HOOK(0x006C8420, C_0085e7b8_AttachWidget, "auto");
