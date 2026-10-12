#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, C_0082d410_AssignWholeString, 0x00433FA0, (C_0082d410* self, void* , int sourceValue));
CSF_FUNC(int, __fastcall, MissionScript_GetEntryCount_46BEA0, 0x0046BEA0, (S_0046bea0* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990, (S_004ff0f0* self, void* , char* String1, char a3));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);

static int __fastcall CMenuSceneSelect_DispatchCommand(
    CMenuSceneSelect* self, void* /*edx*/, int command)
{
    void* linkedControl = reinterpret_cast<void*>(self->field_0014);
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedControl, 0)[0x104 / 4])(
                linkedControl, nullptr);
    if (!result)
        return result;

    void* listBox = reinterpret_cast<void*>(self->field_0050);
    if (!listBox || listBox != CSF_AT(void*, reinterpret_cast<void*>(command), 0x04))
        return self->vHandleLinkedItemChange(command);

    int settings = GetGlobalValue_8BC1E0();

    alignas(8) std::uint8_t scenarioBytes[0x1C];
    C_0082d410& scenario = *reinterpret_cast<C_0082d410*>(scenarioBytes);
    CSF_AT(std::uint32_t, scenarioBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, scenarioBytes, 0x14) = 0;
    CSF_AT(std::uint8_t, scenarioBytes, 0x04) = 0;

    AssignStringSubstring(
        &scenario, nullptr,
        reinterpret_cast<std::uint8_t*>(settings) + 0x48C, 0, -1);

    if (CGuiListBox_GetItemCount_714860(
            reinterpret_cast<CGuiListBox*>(self->field_0050), nullptr) > 0)
    {
        int storedIndex = GetStoredValueAtIndex(
            reinterpret_cast<void*>(self->field_0050), nullptr, 0);
        if (storedIndex != -1)
        {
            std::uint32_t entryIndex = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0050), nullptr, storedIndex, 0);

            // Descriptor reference listed in the packet.
            (void)CSF_VTBL(0x0046BEA0);
            int entryCount = MissionScript_GetEntryCount_46BEA0(
                reinterpret_cast<S_0046bea0*>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x38),
                nullptr);
            if (entryIndex < static_cast<std::uint32_t>(entryCount))
            {
                void* assignedString = AssignStringSubstring(
                    &scenario, nullptr,
                    reinterpret_cast<void*>(self->field_003c + 28 * entryIndex),
                    0, -1);
                C_0082d410_AssignWholeString(
                    reinterpret_cast<C_0082d410*>(
                        reinterpret_cast<std::uint8_t*>(self) + 0x18),
                    nullptr, reinterpret_cast<int>(assignedString));
            }
        }
    }

    if (self->vIsSet_14())
    {
        char* scenarioName = reinterpret_cast<char*>(scenario.field_0004);
        if (CSF_AT(std::uint32_t, scenarioBytes, 0x18) < 0x10)
            scenarioName = reinterpret_cast<char*>(scenarioBytes + 0x04);

        S_004ff0f0* menu =
            reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568());
        GlobalMenu_SetSelectedScenarioName(menu, nullptr, scenarioName, 1);

        if (CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8B0568()), 0x5C) == 1)
        {
            void* controller = reinterpret_cast<void*>(self->field_0034);
            if (controller)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, CMenuSceneSelect*, int)>(
                    CSF_AT(void**, controller, 0)[0x60 / 4])(
                        controller, nullptr, self, 0);

                controller = reinterpret_cast<void*>(self->field_0034);
                const char* menuName = g_pszMenu_P_Juego;
                reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, controller, 0)[0x58 / 4])(
                        controller, nullptr, menuName);
            }
        }
    }

    return CMenuSaveGame_ResetStringStorage(&scenario, nullptr);
}

HOOK(0x0052FC60, CMenuSceneSelect_DispatchCommand, "auto");
