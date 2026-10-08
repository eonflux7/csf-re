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

// CGuiSlider_ForwardSelfToParent: __thiscall, ret 0, tail-jumped with ecx = object.
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x718660, (void* self, void* edx));

// Virtual slot 0xBC (47): returns bool in al, takes no stack args.
static inline uint8_t CallVirtualSlot47IsReady(void* obj) {
    return reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(CSF_AT(void**, obj, 0)[47])(obj, nullptr);
}

// Virtual slot 0xAC (43): returns bool in al, takes no stack args.
static inline uint8_t CallVirtualSlot43IsActive(void* obj) {
    return reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(CSF_AT(void**, obj, 0)[43])(obj, nullptr);
}

static int __fastcall CMenuMpLoadout_ForwardSelectionIfReady(CMenuMpLoadout* self, void* /*edx*/) {
    // First listbox pair: field_0034 (forwarded) gated by field_0028 (checked).
    if (self->field_0034) {
        if (self->field_0028) {
            void* checked = reinterpret_cast<void*>(self->field_0028);
            if (CallVirtualSlot47IsReady(checked)) {
                void* checkedAgain = reinterpret_cast<void*>(self->field_0028);
                if (CallVirtualSlot43IsActive(checkedAgain)) {
                    void* target = reinterpret_cast<void*>(self->field_0034);
                    return CGuiSlider_ForwardSelfToParent(target, nullptr);
                }
            }
        }
    }

    // Second listbox pair: field_0020 (forwarded) gated by field_0030 (checked).
    int result = static_cast<int>(self->field_0020);
    if (result) {
        result = static_cast<int>(self->field_0030);
        if (result) {
            void* checked = reinterpret_cast<void*>(self->field_0030);
            result = CallVirtualSlot47IsReady(checked);
            if (static_cast<uint8_t>(result)) {
                void* checkedAgain = reinterpret_cast<void*>(self->field_0030);
                result = CallVirtualSlot43IsActive(checkedAgain);
                if (static_cast<uint8_t>(result)) {
                    void* target = reinterpret_cast<void*>(self->field_0020);
                    return CGuiSlider_ForwardSelfToParent(target, nullptr);
                }
            }
        }
    }
    return result;
}

HOOK(0x52EF10, CMenuMpLoadout_ForwardSelectionIfReady, "auto");
