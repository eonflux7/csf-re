#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GuiControl_SetInteractionState, 0x0048EEC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00833908_ResetMenuContext, 0x00499080, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_ReleaseCapture, 0x004993E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));

static int __fastcall C_00833908_SetEnabledState(C_00833908* self, void* /*edx*/, int enabled)
{
    const char enabledByte = static_cast<char>(enabled);

    // Original leaves eax unspecified when the state does not change.
    if (CSF_AT(char, self, 0x23C) == enabledByte)
        return 0;

    if (enabledByte != 0)
    {
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x24 / 4])(self, nullptr);

        if (CSF_AT(int, self, 0x1C4) == 1)
        {
            CSF_AT(uint32_t, self, 0x2E0) |= 4;
            CSF_AT(uint32_t, self, 0x2E4) |= 4;
        }
        else
        {
            CSF_AT(uint32_t, self, 0x2E0) &= 0xFFFFFFFB;
            CSF_AT(uint32_t, self, 0x2E4) &= 0xFFFFFFFB;
        }
    }
    else
    {
        if ((CSF_AT(uint32_t, self, 0x2E0) & 0x2000) != 0)
            C_00833908_ReleaseCapture(self, nullptr);

        SetMenuContextState(self, nullptr, 1, 1);
        C_00833908_ResetMenuContext(self, nullptr);
    }

    self->vSlot6C(enabled);
    const int result = GuiControl_SetInteractionState(self, nullptr, enabled);
    CSF_AT(char, self, 0x23C) = enabledByte;
    return result;
}

HOOK(0x0049A7F0, C_00833908_SetEnabledState, "auto");
