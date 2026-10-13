#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, MenuMpPause_AdvanceLayoutPosition, 0x005339B0, (float* position));
CSF_FUNC(int, __cdecl, MenuMpPause_ApplyDisplayRectToCurrentObject, 0x00533AE0, ());
CSF_FUNC(void*, __cdecl, MenuMpPause_LoadTexture, 0x00533D90, (const void* textureName));
CSF_FUNC(std::uint32_t, __fastcall, PushBackFourByteElement, 0x005344D0, (void* self, void* , std::uint32_t* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiButton_Construct, 0x00725390, (CGuiButton* self, void* , int renderer, int context));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static const char* CMenuLanguage_GetEntryText(const void* entry)
{
    if (CSF_AT(std::uint32_t, entry, 0x18) < 0x10)
        return reinterpret_cast<const char*>(entry) + 4;
    return reinterpret_cast<const char*>(CSF_AT(std::uint32_t, entry, 0x4));
}

static int __fastcall CMenuLanguage_RebuildLanguageButtons(
    CMenuLanguage* self, void* /*edx*/, void* screen)
{
    int result = static_cast<int>(self->field_0024);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0024));

    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    if (!self->field_0030 || !screen)
        return result;

    result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x54 / 4])(screen, nullptr);
    if (!result)
        return result;

    const int screenWidth = static_cast<std::int32_t>(g_ScreenWidth);
    const int screenHeight = static_cast<std::int32_t>(g_ScreenHeight);
    void* heightLayout = reinterpret_cast<void*>(GetGlobalValue_753A30());
    void* widthLayout = reinterpret_cast<void*>(GetGlobalValue_753A30());
    const float width = static_cast<float>(screenWidth);
    const float height = static_cast<float>(screenHeight);

    alignas(8) std::uint8_t positionBytes[0xC];
    CSF_AT(float, positionBytes, 0) =
        width * CSF_AT(float, widthLayout, 0x1C4);
    CSF_AT(float, positionBytes, 4) =
        height * CSF_AT(float, heightLayout, 0x1C8);

    void* context = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x54 / 4])(screen, nullptr));
    void* style = reinterpret_cast<void*>(
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, context, 0)[0x48 / 4])(context, nullptr));
    CSF_AT(void*, positionBytes, 8) = style;

    result = static_cast<int>(self->field_0014);
    if (result)
        result = static_cast<std::int32_t>(
            self->field_0018 - self->field_0014) / 28;
    const int count = result;

    if (result != 0)
    {
        std::uint32_t entryOffset = 0;
        std::uint32_t remaining = static_cast<std::uint32_t>(count);
        do
        {
            auto* storage = reinterpret_cast<CGuiButton*>(
                CoreMemory_AllocUntagged(496));
            if (storage)
            {
                context = reinterpret_cast<void*>(
                    reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, screen, 0)[0x54 / 4])(screen, nullptr));
                const int buttonContext =
                    reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, context, 0)[0x10 / 4])(context, nullptr);
                void* controls = MenuControls_ResolveObject(
                    reinterpret_cast<void*>(GetGlobalInt()), nullptr);
                const int constructed = CGuiButton_Construct(
                    storage, nullptr, static_cast<int>(
                        reinterpret_cast<std::uintptr_t>(controls)), buttonContext);

                alignas(8) std::uint8_t buttonSourceBytes[0x1C];
                CSF_AT(std::uint32_t, buttonSourceBytes, 0) =
                    static_cast<std::uint32_t>(constructed);
                auto* button = reinterpret_cast<CGuiButton*>(constructed);
                if (button)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                        button->vtbl[0x174 / 4])(
                            button, nullptr, CSF_AT(std::uint32_t, style, 0xC));
                    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                        button->vtbl[0x178 / 4])(
                            button, nullptr, CSF_AT(std::uint32_t, style, 4));
                    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                        button->vtbl[0x170 / 4])(
                            button, nullptr, CSF_AT(std::uint32_t, style, 0));

                    alignas(8) std::uint8_t locationBytes[0x8];
                    CSF_AT(std::int32_t, locationBytes, 0) =
                        static_cast<std::int32_t>(static_cast<std::int64_t>(
                            CSF_AT(float, positionBytes, 0)));
                    CSF_AT(std::int32_t, locationBytes, 4) =
                        static_cast<std::int32_t>(static_cast<std::int64_t>(
                            CSF_AT(float, positionBytes, 4)));
                    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                        button->vtbl[0x98 / 4])(button, nullptr, locationBytes);

                    heightLayout = reinterpret_cast<void*>(GetGlobalValue_753A30());
                    widthLayout = reinterpret_cast<void*>(GetGlobalValue_753A30());
                    const std::int32_t buttonHeight =
                        static_cast<std::int32_t>(static_cast<std::int64_t>(
                            height * CSF_AT(float, heightLayout, 0x1D0)));
                    alignas(8) std::uint8_t sizeBytes[0x8];
                    CSF_AT(std::int32_t, sizeBytes, 0) =
                        static_cast<std::int32_t>(static_cast<std::int64_t>(
                            width * CSF_AT(float, widthLayout, 0x1CC)));
                    CSF_AT(std::int32_t, sizeBytes, 4) = buttonHeight;
                    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                        button->vtbl[0xA0 / 4])(button, nullptr, sizeBytes);

                    const void* entry = reinterpret_cast<const void*>(
                        self->field_0014 + entryOffset);
                    const char* text = CMenuLanguage_GetEntryText(entry);
                    reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                        button->vtbl[0x70 / 4])(button, nullptr, text);

                    void* displaySetter = button->vtbl[0x1E4 / 4];
                    const int displayRect = MenuMpPause_ApplyDisplayRectToCurrentObject();
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        displaySetter)(button, nullptr, displayRect);

                    void* quadSetter = button->vtbl[0x250 / 4];
                    void* texture = MenuMpPause_LoadTexture(
                        reinterpret_cast<const void*>(self->field_0014 + entryOffset));
                    reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                        quadSetter)(button, nullptr, texture);

                    PushBackFourByteElement(
                        reinterpret_cast<std::uint8_t*>(self) + 0x20, nullptr,
                        reinterpret_cast<std::uint32_t*>(buttonSourceBytes));
                    MenuMpPause_AdvanceLayoutPosition(
                        reinterpret_cast<float*>(positionBytes));
                    style = CSF_AT(void*, positionBytes, 8);
                }
            }
            entryOffset += 28;
            --remaining;
        } while (remaining != 0);
        result = count;
    }

    if (result > 0)
    {
        std::uint32_t previousIndex = static_cast<std::uint32_t>(result - 1);
        std::uint32_t buttonOffset = 0;
        int remaining = result;
        do
        {
            const int nextIndex =
                static_cast<std::int32_t>(previousIndex + 2) % count;
            void* button = CSF_AT(void*,
                reinterpret_cast<void*>(self->field_0024 + buttonOffset), 0);
            const int previousEntry = static_cast<std::int32_t>(previousIndex) % count;
            const char* previousText = CMenuLanguage_GetEntryText(
                reinterpret_cast<const void*>(self->field_0014 +
                    static_cast<std::uint32_t>(previousEntry) * 28));
            reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, button, 0)[0x78 / 4])(button, nullptr, previousText);

            const char* nextText = CMenuLanguage_GetEntryText(
                reinterpret_cast<const void*>(self->field_0014 +
                    static_cast<std::uint32_t>(nextIndex) * 28));
            button = CSF_AT(void*,
                reinterpret_cast<void*>(self->field_0024 + buttonOffset), 0);
            reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, button, 0)[0x7C / 4])(button, nullptr, nextText);

            button = CSF_AT(void*,
                reinterpret_cast<void*>(self->field_0024 + buttonOffset), 0);
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, button, 0)[0x108 / 4])(
                    button, nullptr, self->field_0030);

            buttonOffset += 4;
            result = remaining - 1;
            ++previousIndex;
            --remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x005345D0, CMenuLanguage_RebuildLanguageButtons, "auto");
