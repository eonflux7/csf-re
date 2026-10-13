#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static uint32_t MenuVideoOptions_FindControl(uint32_t parent, const char* name)
{
    void* object = reinterpret_cast<void*>(parent);
    return reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, object, 0)[252 / 4])(object, nullptr, name);
}

static void MenuVideoOptions_ClearChoices(uint32_t control)
{
    void* object = reinterpret_cast<void*>(control);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[696 / 4])(object, nullptr);
}

static int MenuVideoOptions_AddLocalizedChoice(uint32_t control, int textId)
{
    void* object = reinterpret_cast<void*>(control);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, object, 0)[668 / 4])(object, nullptr, textId);
}

static void MenuVideoOptions_AddChoiceData(uint32_t control, const void* data)
{
    void* object = reinterpret_cast<void*>(control);
    reinterpret_cast<void(__fastcall*)(void*, void*, const void*)>(
        CSF_AT(void**, object, 0)[664 / 4])(object, nullptr, data);
}

static void MenuVideoOptions_SetOptionState(uint32_t control, int state,
                                           int flag1, int flag2)
{
    void* object = reinterpret_cast<void*>(control);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, object, 0)[104 / 4])(object, nullptr, state, flag1, flag2);
}

static int __fastcall CMenuVideoAdvanced_ResetOptionState(
    CMenuVideoAdvanced* self, void* /*edx*/)
{
    self->field_0028 = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843FA0, "VerticalSync"));
    if (self->field_0028)
    {
        MenuVideoOptions_ClearChoices(self->field_0028);
        MenuVideoOptions_AddLocalizedChoice(self->field_0028, 1835170149);
        MenuVideoOptions_AddLocalizedChoice(self->field_0028, 1835167343);
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        MenuVideoOptions_SetOptionState(
            self->field_0028, CSF_AT(uint8_t, settings, 0xCC), 1, 1);
    }

    self->field_002c = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843F90, "Antialiasing"));
    if (self->field_002c)
    {
        MenuVideoOptions_ClearChoices(self->field_002c);
        MenuVideoOptions_AddLocalizedChoice(self->field_002c, 1633955895);
        MenuVideoOptions_AddChoiceData(
            self->field_002c, reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00843F8C)));
        MenuVideoOptions_AddChoiceData(
            self->field_002c, reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00843F88)));
        MenuVideoOptions_AddChoiceData(
            self->field_002c, reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00843F84)));
        MenuVideoOptions_AddChoiceData(
            self->field_002c, reinterpret_cast<const void*>(CSF_ADDR_DATA(0x00843F80)));
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        MenuVideoOptions_SetOptionState(
            self->field_002c, CSF_AT(uint8_t, settings, 0xCC), 1, 1);
    }

    self->field_0030 = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843F78, "Shadows"));
    if (self->field_0030)
    {
        MenuVideoOptions_ClearChoices(self->field_0030);
        MenuVideoOptions_AddLocalizedChoice(self->field_0030, 1633955895);
        MenuVideoOptions_AddLocalizedChoice(self->field_0030, 1982869559);
        MenuVideoOptions_AddLocalizedChoice(self->field_0030, 1982869560);
        MenuVideoOptions_AddLocalizedChoice(self->field_0030, 1982869561);
    }

    self->field_0034 = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843F68, "TextureFilter"));
    if (self->field_0034)
    {
        MenuVideoOptions_ClearChoices(self->field_0034);
        MenuVideoOptions_AddLocalizedChoice(self->field_0034, 1982869559);
        MenuVideoOptions_AddLocalizedChoice(self->field_0034, 1982869560);
        MenuVideoOptions_AddLocalizedChoice(self->field_0034, 1982869561);
    }

    self->field_0038 = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843F58, "TextureQuality"));
    if (self->field_0038)
    {
        MenuVideoOptions_ClearChoices(self->field_0038);
        MenuVideoOptions_AddLocalizedChoice(self->field_0038, 1982869559);
        MenuVideoOptions_AddLocalizedChoice(self->field_0038, 1982869560);
        MenuVideoOptions_AddLocalizedChoice(self->field_0038, 1982869561);
        void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
        MenuVideoOptions_SetOptionState(
            self->field_0038, CSF_AT(uint8_t, settings, 0xCC), 1, 1);
    }

    self->field_003c = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843F48, "ModelQuality"));
    if (self->field_003c)
    {
        MenuVideoOptions_ClearChoices(self->field_003c);
        MenuVideoOptions_AddLocalizedChoice(self->field_003c, 1982869559);
        MenuVideoOptions_AddLocalizedChoice(self->field_003c, 1982869560);
        MenuVideoOptions_AddLocalizedChoice(self->field_003c, 1982869561);
    }

    self->field_0040 = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843F38, "EffectsQuality"));
    if (self->field_0040)
    {
        MenuVideoOptions_ClearChoices(self->field_0040);
        MenuVideoOptions_AddLocalizedChoice(self->field_0040, 1982869559);
        MenuVideoOptions_AddLocalizedChoice(self->field_0040, 1982869560);
        MenuVideoOptions_AddLocalizedChoice(self->field_0040, 1982869561);
    }

    self->field_0044 = MenuVideoOptions_FindControl(
        self->field_001c, CSF_STR(0x00843F28, "WaterQuality"));
    if (self->field_0044)
    {
        MenuVideoOptions_ClearChoices(self->field_0044);
        MenuVideoOptions_AddLocalizedChoice(self->field_0044, 1982869559);
        MenuVideoOptions_AddLocalizedChoice(self->field_0044, 1982869560);
        return MenuVideoOptions_AddLocalizedChoice(self->field_0044, 1982869561);
    }
    return 0;
}

HOOK(0x00548D20, CMenuVideoAdvanced_ResetOptionState, "auto");
