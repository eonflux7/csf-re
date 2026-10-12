#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CheatsMenu_ClearFieldAndRefresh, 0x0042CBD0, (S_0042cbd0* self, void* , int a2, int));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void*));
CSF_FUNC(int, __fastcall, S_00536500_PrepareCharacterSelection, 0x00536500, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00830578_SetCharacterSelectionActive, 0x00536650, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_CommitCharacterSelection, 0x00542820, ());

// The packet additionally records these code addresses as vtable references.
[[maybe_unused]] static const auto g_ReportedCheatsMenuVtable = CSF_VTBL(0x0042CBD0);
[[maybe_unused]] static const auto g_ReportedCharacterSelectionVtable = CSF_VTBL(0x00536500);

static char __fastcall S_0042cbd0_HandleCharacterSelectionInput(
    S_0042cbd0* self, void* /*edx*/, int input, int argument)
{
    const char selectionActive = static_cast<char>(self->field_013d);
    if (selectionActive)
    {
        if (input != 28)
            return selectionActive;

        void* child = CSF_AT(void*, self, 0x134);
        if (child)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, child, 0)[0xA8 / 4])(child, nullptr, 1);

            child = CSF_AT(void*, self, 0x134);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x58 / 4])(child, nullptr);
        }

        void* owner = reinterpret_cast<std::uint8_t*>(self) - 4;
        self->field_013d = 0;
        C_00830578_SetCharacterSelectionActive(owner, nullptr, 1);
        CSF_AT(int, self, 0x130) = 2;
        MenuMpCharacterSelect_CommitCharacterSelection();

        const int globalState = GetGlobalState_50D190();
        MenuSystem_ToggleChildFlag(reinterpret_cast<void*>(globalState), nullptr);

        return ((char(__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0xB8 / 4])(owner, nullptr);
    }

    CheatsMenu_ClearFieldAndRefresh(self, nullptr, input, argument);
    const int globalState = GetGlobalState_50D190();
    if (!CSF_AT(std::uint8_t, reinterpret_cast<void*>(globalState), 0xD0))
        return static_cast<char>(globalState);

    const char isSelected = static_cast<char>(self->field_0144 == 1);
    if (!isSelected || input != 1)
        return isSelected;

    void* owner = reinterpret_cast<std::uint8_t*>(self) - 4;
    S_00536500_PrepareCharacterSelection(owner, nullptr);
    return ((char(__fastcall*)(void*, void*, int))
        CSF_AT(void**, owner, 0)[0x54 / 4])(owner, nullptr, 1);
}

HOOK(0x005366F0, S_0042cbd0_HandleCharacterSelectionInput, "auto");
