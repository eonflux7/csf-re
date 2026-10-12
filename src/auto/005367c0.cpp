#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CheatsMenu_SetFieldModeAndRefresh, 0x0042CD20, (S_0042cd20* self, void* , int a2, int));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void*));
CSF_FUNC(int, __fastcall, Interface_PrepareCharacterSelection, 0x00536500, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Interface_SetCharacterSelectionActive, 0x00536650, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_CommitCharacterSelection, 0x00542820, ());
CSF_FUNC(int, __cdecl, Sub_7A8ED0, 0x007A8ED0, (int a2));

static char __fastcall S_0042cd20_HandleCharacterSelection(
    S_0042cd20* self, void* /*edx*/, int mode, int refreshArgument)
{
    // The packet also identifies these method addresses as vtable references.
    (void)CSF_VTBL(0x0042CD20);
    (void)CSF_VTBL(0x00536500);

    const char active = static_cast<char>(self->field_013d);
    if (active)
    {
        if (mode != 5)
            return active;

        void* owner = reinterpret_cast<uint8_t*>(self) - 4;
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[176 / 4])(
            owner, nullptr);

        void* child = CSF_AT(void*, self, 0x134);
        if (child)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[168 / 4])(
                child, nullptr, 1);

            child = CSF_AT(void*, self, 0x134);
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[88 / 4])(
                child, nullptr);
        }

        self->field_013d = 0;
        Interface_SetCharacterSelectionActive(owner, nullptr, 0);
        CSF_AT(int, self, 0x130) = 2;
        MenuMpCharacterSelect_CommitCharacterSelection();

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        MenuSystem_ToggleChildFlag(globalState, nullptr);

        return ((char(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[184 / 4])(
            owner, nullptr);
    }

    CheatsMenu_SetFieldModeAndRefresh(self, nullptr, mode, refreshArgument);
    const int globalState = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xD0))
        return static_cast<char>(globalState);

    if (self->field_0144 != 1)
        return 0;

    const char eligible = static_cast<char>(Sub_7A8ED0(mode));
    if (!eligible)
        return eligible;

    void* owner = reinterpret_cast<uint8_t*>(self) - 4;
    Interface_PrepareCharacterSelection(owner, nullptr);
    return ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, owner, 0)[84 / 4])(
        owner, nullptr, 1);
}

HOOK(0x005367C0, S_0042cd20_HandleCharacterSelection, "auto");
