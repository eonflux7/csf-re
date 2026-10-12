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

CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_IsSecondaryStateActive, 0x005427F0, ());

// Slot 0xA4 / 4 = 41 on the object's vtable; called as a __thiscall with this in ecx.
static char __fastcall SceneInstancing_CommitSecondaryState(uint8_t* self, void* /*edx*/)
{
    int secondaryActive = MenuMpCharacterSelect_IsSecondaryStateActive();
    if (!secondaryActive)
        return 0;

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
    CSF_AT(int, self, 0x148) = 2;
    CSF_AT(uint8_t, self, 0x141) = 1;
    CSF_AT(uint8_t, self, 0x140) = 1;
    return 1;
}

HOOK(0x00536560, SceneInstancing_CommitSecondaryState, "auto");
