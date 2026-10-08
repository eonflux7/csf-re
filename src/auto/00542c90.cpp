#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callees, bound by address.
// 0x506BD0: GetGlobalValue_8B056C, __cdecl, no args, returns pointer in eax.
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());
// 0x42AF90: sub_42AF90, caller-cleaned (add esp,0xc). Original also takes ebx and esi
// as register inputs, which this signature cannot express (see notes).
CSF_FUNC(void, __cdecl, SetSelectionRefresh_42AF90, 0x42AF90, (uint32_t a3, uint32_t a4, uint32_t a5));
// 0x716A60: CGuiListBoxBase_AddSelectedItem, __thiscall, one stack arg, ret 4.
// Its eax on return is passed through as this function's result.
CSF_FUNC(int, __fastcall, GuiListBoxBase_AddSelectedItem, 0x716A60, (void* self, void* edx, int item));

// Refreshes the selection in the menu controls and adds a selected item to the list box.
static int __fastcall MenuControls_RefreshSelectionAndAddItem(void* self, void* /*edx*/)
{
    uint8_t* ctl = reinterpret_cast<uint8_t*>(self);

    // Notify the global object's child (vtable slot 3) if present.
    uint8_t* globalObj = reinterpret_cast<uint8_t*>(GetGlobalValue_8B056C());
    void* notifyTarget = CSF_AT(void*, globalObj, 0x1C);
    if (notifyTarget) {
        using NotifyFn = void(__fastcall*)(void*, void*, void*, int);
        NotifyFn notify = reinterpret_cast<NotifyFn>(CSF_AT(void**, notifyTarget, 0)[3]);
        notify(notifyTarget, nullptr, ctl + 0x48, 0);
    }

    // sub_42AF90(a3 = [ctl+0x2C8], a4 = ctl+0x48, a5 = [ctl+0x2C]); a2 (ebx) is not expressible here.
    SetSelectionRefresh_42AF90(CSF_AT(uint32_t, ctl, 0x2C8),
                               static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ctl + 0x48)),
                               CSF_AT(uint32_t, ctl, 0x2C));

    // Add item 0 to the list box at [ctl+0x2C]; return whatever eax the callee leaves.
    void* listBox = reinterpret_cast<void*>(CSF_AT(uintptr_t, ctl, 0x2C));
    return GuiListBoxBase_AddSelectedItem(listBox, nullptr, 0);
}

HOOK(0x542C90, MenuControls_RefreshSelectionAndAddItem, "auto");
