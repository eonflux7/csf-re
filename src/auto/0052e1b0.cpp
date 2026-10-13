#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Hud_WidgetTextEquals, 0x00431CB0, (int a1, const char* a2));
CSF_FUNC(int, __cdecl, FxParticleManager_HasNamedEffect, 0x00437D60, (int a1, const char* a2));
CSF_FUNC(int, __fastcall, CMenuScenarioSelect_ForwardAction, 0x0052D720, (void* self, void* , int action));
CSF_FUNC(void*, __fastcall, CMenuScenarioSelect_ActivateSelectedScenario, 0x0052E050, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszMenu_P_SShots, 0x00899ADC);
CSF_GLOBAL(const char*, g_pszMenu_P_LoMenu, 0x00899B0C);

static void* __fastcall CMenuScenarioSelect_DispatchCommand(
    CMenuScenarioSelect* self, void* /*edx*/, int command)
{
    void* controller = reinterpret_cast<void*>(self->field_0018);
    void* result =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x104 / 4])(
                controller, nullptr);
    if (!result)
        return result;

    void* event = reinterpret_cast<void*>(command);
    if ((self->field_0054 &&
         self->field_0054 == CSF_AT(std::uint32_t, event, 0x04)) ||
        (self->field_0058 &&
         self->field_0058 == CSF_AT(std::uint32_t, event, 0x04)))
    {
        void* scenarioName = reinterpret_cast<std::uint8_t*>(self) + 0x1C;
        AssignStringRange_429B60(
            scenarioName, nullptr,
            const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);

        if (self->field_003c != static_cast<std::uint32_t>(-1))
        {
            AssignStringSubstring(
                scenarioName, nullptr,
                reinterpret_cast<const void*>(
                    self->field_0048 + 196 * self->field_003c + 32),
                0, -1);

            const void* source;
            if (static_cast<std::uint8_t>(Hud_WidgetTextEquals(
                    reinterpret_cast<int>(scenarioName),
                    CSF_STR(0x0082E65C, ""))))
            {
                source = reinterpret_cast<const void*>(
                    self->field_0048 + 196 * self->field_003c + 4);
            }
            else
            {
                source = scenarioName;
            }

            AssignStringSubstring(scenarioName, nullptr, source, 0, -1);
        }

        const std::uint8_t canActivate = self->vIsSet_17();
        result = reinterpret_cast<void*>(
            static_cast<std::uintptr_t>(canActivate));
        if (canActivate)
        {
            const int hasName = FxParticleManager_HasNamedEffect(
                reinterpret_cast<int>(scenarioName),
                CSF_STR(0x0082E65C, ""));
            result = reinterpret_cast<void*>(hasName);
            if (static_cast<std::uint8_t>(hasName))
                return CMenuScenarioSelect_ActivateSelectedScenario(
                    self, nullptr);
        }
        return result;
    }

    if (self->field_005c &&
        self->field_005c == CSF_AT(std::uint32_t, event, 0x04))
    {
        CMenuScenarioSelect_ForwardAction(self, nullptr, 0);
        void* menuManager = reinterpret_cast<void*>(self->field_0014);
        return reinterpret_cast<void* (__fastcall*)(
            void*, void*, const char*)>(
                CSF_AT(void**, menuManager, 0)[0x58 / 4])(
                    menuManager, nullptr, g_pszMenu_P_LoMenu);
    }

    if (self->field_0060 &&
        CSF_AT(std::uint32_t, event, 0x04) == self->field_0060)
    {
        CMenuScenarioSelect_ForwardAction(self, nullptr, 0);
        void* menuManager = reinterpret_cast<void*>(self->field_0014);
        return reinterpret_cast<void* (__fastcall*)(
            void*, void*, const char*)>(
                CSF_AT(void**, menuManager, 0)[0x58 / 4])(
                    menuManager, nullptr, g_pszMenu_P_SShots);
    }

    return reinterpret_cast<void*>(self->vHandleLinkedItemChange(command));
}

HOOK(0x0052E1B0, CMenuScenarioSelect_DispatchCommand, "auto");
