#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(char, __fastcall, CharActionFsm_SetCharacterToggleState, 0x004975E0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, Hud_ResetSubObjectSlot, 0x004A1260, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GlobalValue_SetSelection, 0x004FC3B0, (void* self, void* edx, int a2));

// Clears the pending HUD action when the action-pending flag (0x40) is set and the
// linked object reports sub-state 2: resets the toggle state, resets the sub-object
// slot, re-activates the linked object and clears the global selection.
static void __fastcall Hud_ClearPendingAction(C_00834050* self, void* /*edx*/)
{
    if ((CSF_AT(uint8_t, self, 0x2E0) & 0x40) == 0)
        return;

    void* hudObj = reinterpret_cast<void*>(self->field_0044);
    using GetLinkedFn = void* (__fastcall*)(void*, void*);
    void* linkedRaw = ((GetLinkedFn)CSF_AT(void**, hudObj, 0)[54])(hudObj, nullptr);
    C_008319c0* linked = reinterpret_cast<C_008319c0*>(linkedRaw);
    if (!linked)
        return;

    if (C_008319c0_GetSubObjectField74(linked, nullptr) != 2)
        return;

    CharActionFsm_SetCharacterToggleState(self, nullptr, 0);
    Hud_ResetSubObjectSlot(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x344), nullptr, 0);

    void* activeRaw = ((GetLinkedFn)CSF_AT(void**, hudObj, 0)[54])(hudObj, nullptr);
    using SetActiveFn = void (__fastcall*)(void*, void*, int);
    ((SetActiveFn)CSF_AT(void**, activeRaw, 0)[12])(activeRaw, nullptr, 1);

    // The original pushes -1 before calling the getter; it stays on the stack and is
    // consumed by GlobalValue_SetSelection (ret 4), so it is passed as its argument here.
    void* globalValue = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    GlobalValue_SetSelection(globalValue, nullptr, -1);
}

HOOK(0x00497750, Hud_ClearPendingAction, "auto");
