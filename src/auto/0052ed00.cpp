#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// CGuiSlider_ForwardSelfToParent: __thiscall, no stack args (ret 0).
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));

// Looks up a child control by name through the owning menu object (this->field_0018),
// using its virtual slot 63 (byte offset 0xFC).
static uint32_t FindMenuControl(CMenuMpLoadout* self, const char* controlName)
{
    void* menu = reinterpret_cast<void*>(self->field_0018);
    return reinterpret_cast<uint32_t>(
        ((void*(__fastcall*)(void*, void*, const char*))CSF_AT(void**, menu, 0)[63])(menu, nullptr, controlName));
}

// Sends a command to the team listbox (this->field_0020) through its virtual slot 167 (byte offset 0x29C).
static void SendTeamListboxCommand(CMenuMpLoadout* self, uint32_t command)
{
    void* listbox = reinterpret_cast<void*>(self->field_0020);
    ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, listbox, 0)[167])(listbox, nullptr, command);
}

// Caches the child controls of the multiplayer loadout screen. Sets the primary weapon
// listbox's +0x204 field to 8, sends three commands to the team listbox, and returns the
// result of CGuiSlider_ForwardSelfToParent on the team listbox, or the play button handle
// when the team listbox is absent.
static int __fastcall CMenuMpLoadout_BindChildControls(CMenuMpLoadout* self, void* /*edx*/)
{
    self->field_001c = FindMenuControl(self, CSF_STR(0x8417B0, "TitleTeam"));
    self->field_0020 = FindMenuControl(self, CSF_STR(0x8417A4, "ListboxTeam"));
    self->field_0024 = FindMenuControl(self, CSF_STR(0x841798, "TitleClass"));
    self->field_0028 = FindMenuControl(self, CSF_STR(0x841788, "ListboxClass"));
    self->field_002c = FindMenuControl(self, CSF_STR(0x841774, "TitlePrimaryWeapon"));

    uint32_t listboxPrimaryWeapon = FindMenuControl(self, CSF_STR(0x84175C, "ListboxPrimaryWeapon"));
    self->field_0030 = listboxPrimaryWeapon;
    if (listboxPrimaryWeapon != 0)
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(listboxPrimaryWeapon), 0x204) = 8;
    }

    uint32_t playButton = FindMenuControl(self, CSF_STR(0x841724, "BotonPlay"));
    self->field_0034 = playButton;

    if (self->field_0020 != 0)
    {
        SendTeamListboxCommand(self, 0x494E3032u);
        SendTeamListboxCommand(self, 0x494E3033u);
        SendTeamListboxCommand(self, 0x494E3034u);
        return CGuiSlider_ForwardSelfToParent(reinterpret_cast<void*>(self->field_0020), nullptr);
    }

    return static_cast<int>(playButton);
}

HOOK(0x0052ED00, CMenuMpLoadout_BindChildControls, "auto");
