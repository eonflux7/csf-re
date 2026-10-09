#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, MenuMissionInfo_IsDefaultDestinationFlagSet, 0x0053D690, ());
CSF_FUNC(int, __fastcall, CMenuMissionInfo_EnterPanelState, 0x0053D740, (CMenuMissionInfo* self, void* , int));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ProcessPendingPageStep, 0x0053E9A0, (CMenuMissionInfo* self, void*));
CSF_FUNC(char, __fastcall, CMenuMissionInfo_TryAdvanceToNextPage, 0x0053E9E0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(const char*, g_pszMenu_P_LoGame, 0x00899B08);

static void __fastcall C_0085ec18_HandleMenuInput(
    C_0085ec18* self, void* /*edx*/, char* eventCode, int eventData)
{
    auto* panel = reinterpret_cast<CMenuMissionInfo*>(
        reinterpret_cast<std::uint8_t*>(self) - 4);
    const auto code = reinterpret_cast<std::uintptr_t>(eventCode);

    if (CSF_AT(std::uint8_t, CSF_AT(void*, self, 0x10), 0x04))
    {
        void* global = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* inputState = CSF_AT(void*, global, 0x1C);
        if (CSF_AT(std::uint32_t, inputState, 0x1D0) == 0 &&
            eventCode == CSF_AT(char*, inputState, 0x1D4))
        {
            CMenuMissionInfo_EnterPanelState(panel, nullptr, 1);
            return;
        }
    }
    else
    {
        switch (code)
        {
        case 1:
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, panel, 0)[16])(panel, nullptr, 1);
            return;

        case 0x1C:
        case 0x9C:
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, panel, 0)[16])(panel, nullptr, 0);
            return;

        case 0x39:
            if (CSF_AT(std::uint8_t,
                       reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC) &&
                static_cast<std::uint8_t>(
                    MenuMissionInfo_IsDefaultDestinationFlagSet()))
            {
                void* destination = CSF_AT(void*, self, 0x0C);
                const char* menuName = g_pszMenu_P_LoGame;
                reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, destination, 0)[22])(
                        destination, nullptr, menuName);
                return;
            }
            break;

        default:
            break;
        }
    }

    switch (code)
    {
    case 50:
        if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(eventData), 0x38))
        {
            void* referencedObject = GetReferencedObject(self, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, referencedObject, 0)[7])(
                    referencedObject, nullptr);
        }
        break;

    case 203:
        CMenuMissionInfo_ProcessPendingPageStep(panel, nullptr);
        break;

    case 205:
        CMenuMissionInfo_TryAdvanceToNextPage(panel, nullptr);
        break;

    default:
        break;
    }
}

HOOK(0x0053FDB0, C_0085ec18_HandleMenuInput, "auto");
