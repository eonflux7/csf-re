#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CMenuMpGameMode_SetBackButton: __thiscall(this, a2), ret 4
CSF_FUNC(int, __fastcall, CMenuMpGameMode_SetBackButton, 0x0054ABA0, (void* self, void* edx, int a2));

// Original: 0x0053ABB0. Runs the back-button setup, calls the vtable slot at +0x30
// (result ignored: both original branches are identical), then looks up "BotonControles"
// on the object stored at +0x1C via vtable slot +0xFC and stores the result at +0x20.
static int __fastcall CMenuOptionsPC_InitControlsButton(CMenuOptionsPC* self, void* /*edx*/, int a2)
{
    CMenuMpGameMode_SetBackButton(self, nullptr, a2);

    // Virtual call at vtable +0x30 on self; the result is not used by the original.
    ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr);

    void* controlsOwner = reinterpret_cast<void*>(self->field_001c);
    int result = ((int(__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, controlsOwner, 0)[0xFC / 4])(
                        controlsOwner, nullptr, CSF_STR(0x008425D8, "BotonControles"));

    self->field_0020 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x0053ABB0, CMenuOptionsPC_InitControlsButton, "auto");
