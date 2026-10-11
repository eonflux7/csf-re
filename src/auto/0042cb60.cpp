#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Input-state test on the menu's controller/flag block (field_0014).
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(bool, __fastcall, MenuControls_IsActionActive, 0x004285B0, (void* self, void* edx, int a2));

// Per-frame helpers called at the end of the frame update.
CSF_FUNC(void, __fastcall, CheatMenu_UpdateItemsTick, 0x0042C3B0, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, CheatMenu_UpdateCursorTick, 0x0042C7F0, (void* self, void* edx, float a2));

// Per-frame update for the cheats menu: handles two input flags, then ticks
// the item list and cursor with the frame delta.
static void __fastcall C_00830578_ProcessInputFrame(C_00830578* self, void* /*edx*/, float frameDelta)
{
    if (MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 92))
    {
        // Slot 0x64 has no typed wrapper; call it with argument 1.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x64 / 4])(self, nullptr, 1);
    }

    if (MenuControls_IsActionActive(reinterpret_cast<void*>(self->field_0014), nullptr, 36))
    {
        self->vRunActionChecked();
    }

    if (MenuControls_IsActionActive(reinterpret_cast<void*>(self->field_0014), nullptr, 37))
    {
        self->vCanEnterMenuState();
    }

    CheatMenu_UpdateCursorTick(self, nullptr, frameDelta);
    CheatMenu_UpdateItemsTick(self, nullptr, frameDelta);
}

HOOK(0x0042CB60, C_00830578_ProcessInputFrame, "auto");
