#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0053CC50 (16 bytes, ret 0)
// Reads the current value from the control object stored at +0x2C by calling its
// virtual method at vtable offset +0x28, stores it into field_0038 and returns it.
static int __fastcall CMenuSoundOptions_StoreControlValue(CMenuSoundOptions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_002c);
    void** controlVtbl = *reinterpret_cast<void***>(control);
    int value = reinterpret_cast<int(__fastcall*)(void*, void*)>(controlVtbl[0x28 / 4])(control, nullptr);
    self->field_0038 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x0053CC50, CMenuSoundOptions_StoreControlValue, "auto");
