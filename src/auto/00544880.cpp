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

// Virtual slots used (byte offset / 4):
//   0x68 -> 26, 0x6c -> 27, 0x274 -> 157

// Forward to CGuiSlider::ForwardSelfToParent (thiscall, no stack args, ret 0).
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));

// Virtual call helpers (thiscall via fastcall shim; edx unused).
static int CallVirtualNoArgs(void* obj, int slot) {
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, obj, 0)[slot])(obj, nullptr);
}

static void CallVirtualThreeArgs(void* obj, int slot, int a, int b, int c) {
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(CSF_AT(void**, obj, 0)[slot])(obj, nullptr, a, b, c);
}

// Refreshes the save-game menu buttons depending on the state of the three child objects
// stored at +0x14, +0x10 and +0x1C. Returns the value of the last inspected call, or 0
// when any of the three referenced objects is missing.
static int __fastcall CMenuSaveGame_RefreshButtonsState(CMenuSaveGame* self, void* /*edx*/) {
    void* primary = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    void* listbox = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0010));
    void* secondary = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_001c));

    if (!primary || !listbox || !secondary) {
        return 0;
    }

    int result = 0;
    if (!CallVirtualNoArgs(primary, 27) || CallVirtualNoArgs(secondary, 157)) {
        result = CallVirtualNoArgs(primary, 27);
        if (!static_cast<uint8_t>(result)) {
            result = CallVirtualNoArgs(secondary, 157);
            if (result > 0) {
                CallVirtualThreeArgs(primary, 26, 1, 1, 1);
            }
        }
        return result;
    }

    CallVirtualThreeArgs(primary, 26, 0, 1, 1);
    return CGuiSlider_ForwardSelfToParent(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0010)), nullptr);
}

HOOK(0x00544880, CMenuSaveGame_RefreshButtonsState, "auto");
