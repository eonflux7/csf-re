#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_MenuOptionText, 0x00899ACC);
CSF_GLOBAL(const char*, g_MenuOptionsInGameText, 0x00899AD4);

static int __fastcall CMenuSoundOptions_RestoreSettingsAndConfigureNavigation(
    CMenuSoundOptions* self, void* /*edx*/)
{
    void* settings = reinterpret_cast<void*>(self->field_002c);
    if (settings != nullptr)
    {
        std::uint32_t savedValue;
        std::memcpy(&savedValue, &self->field_0030, sizeof(savedValue));
        ((void(__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, settings, 0)[0x1C / 4])(
                settings, nullptr, savedValue);

        settings = reinterpret_cast<void*>(self->field_002c);
        std::memcpy(&savedValue, &self->field_0034, sizeof(savedValue));
        ((void(__fastcall*)(void*, void*, std::uint32_t))
            CSF_AT(void**, settings, 0)[0x24 / 4])(
                settings, nullptr, savedValue);

        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x70 / 4])(self, nullptr);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x74 / 4])(self, nullptr);
    }

    void* navigation = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*, CMenuSoundOptions*, int))
        CSF_AT(void**, navigation, 0)[0x60 / 4])(
            navigation, nullptr, self, 0);

    const std::uint32_t existingState = self->field_0018;
    if (existingState != 0)
        return static_cast<int>(existingState);

    const std::uint8_t inGame = ((std::uint8_t(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr);

    const char* text = g_MenuOptionsInGameText;
    if (inGame == 0)
        text = g_MenuOptionText;

    navigation = reinterpret_cast<void*>(self->field_0014);
    return ((int(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, navigation, 0)[0x58 / 4])(
            navigation, nullptr, text);
}

HOOK(0x0053CD30, CMenuSoundOptions_RestoreSettingsAndConfigureNavigation, "auto");
