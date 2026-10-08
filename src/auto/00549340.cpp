#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// int __thiscall sub_5491A0(int* this, int a2)  -- ret 4
CSF_FUNC(int, __fastcall, CMenuVideoAdvanced_ApplyChangedOption, 0x5491A0,
         (CMenuVideoAdvanced* self, void* /*edx*/, uint32_t* a2));

// Handles a menu event: for event id 13 the video-option change is dispatched and the
// event is then forwarded to the vtable slot at +0x48 (index 18); otherwise the raw
// (id - 13) is returned.
static int __fastcall CMenuVideoAdvanced_HandleEvent(CMenuVideoAdvanced* self, void* /*edx*/, uint32_t* a2)
{
    int result = static_cast<int>(*a2 - 13u);

    if (*a2 == 13u)
    {
        CMenuVideoAdvanced_ApplyChangedOption(self, nullptr, a2);

        // vtable pointer is re-read after the call (the callee may replace it)
        void** vtbl = *reinterpret_cast<void***>(self);
        return ((int(__fastcall*)(void*, void*, uint32_t*))vtbl[0x48 / 4])(self, nullptr, a2);
    }

    return result;
}

HOOK(0x549340, CMenuVideoAdvanced_HandleEvent, "auto");
