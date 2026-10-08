#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// void __thiscall CMenuSoundOptions::DispatchEvent13(CMenuSoundOptions* self, uint32_t* event)
CSF_FUNC(int, __fastcall, CMenuSoundOptions_HandleVolumeSliderEvent, 0x53C9E0,
         (CMenuSoundOptions* self, void* /*edx*/, uint32_t* a2));

// Handles a menu event: for event id 13 the sound-option change is dispatched and then
// forwarded to the vtable slot at +0x48 (index 18); otherwise the raw (id - 13) is returned.
static int __fastcall CMenuSoundOptions_HandleEvent(CMenuSoundOptions* self, void* /*edx*/, uint32_t* a2)
{
    int result = static_cast<int>(*a2 - 13u);

    if (*a2 == 13u)
    {
        CMenuSoundOptions_HandleVolumeSliderEvent(self, nullptr, a2);

        // vtable is re-read after the call (the callee may replace it)
        void** vtbl = *reinterpret_cast<void***>(self);
        return ((int(__fastcall*)(void*, void*, uint32_t*))vtbl[0x48 / 4])(self, nullptr, a2);
    }

    return result;
}

HOOK(0x53CF30, CMenuSoundOptions_HandleEvent, "auto");
