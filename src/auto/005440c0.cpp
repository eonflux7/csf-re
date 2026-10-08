#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// void __thiscall sub_53B510(this) : recomputes the screen layout before the
// slider widgets are queried. Called with ecx == the menu screen object.
CSF_FUNC(void, __fastcall, CMenuGameOptions_SyncOptionFlagsFromSource, 0x53B510,
         (CMenuGameOptions* self, void* /*edx*/));

// Re-reads the slider widget state: pulls its current value (virtual slot at
// vtable+0x34, returns a float) into field_005c and its checked/active flag
// (virtual slot at vtable+0x60, returns an int) into field_0060.
static int __fastcall CMenuGameOptionsPC_SyncSliderFromControl(CMenuGameOptionsPC* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0024);
    if (result)
    {
        CMenuGameOptions_SyncOptionFlagsFromSource(self, nullptr);

        void* widget = reinterpret_cast<void*>(self->field_0024);
        void** vtbl = *reinterpret_cast<void***>(widget);
        float value = reinterpret_cast<float(__fastcall*)(void*, void*)>(vtbl[0x34 / 4])(widget, nullptr);
        self->field_005c = value;

        // Reload the widget pointer: the previous virtual call may have changed it.
        widget = reinterpret_cast<void*>(self->field_0024);
        vtbl = *reinterpret_cast<void***>(widget);
        result = reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[0x60 / 4])(widget, nullptr);
        self->field_0060 = static_cast<uint8_t>(result);
    }
    return result;
}

HOOK(0x5440C0, CMenuGameOptionsPC_SyncSliderFromControl, "auto");
