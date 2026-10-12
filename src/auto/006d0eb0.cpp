#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Reads a state code from the object's virtual slot 7 (offset 0x1C) and stores a
// mode byte at provider+2. States 0/3 store 3, states 1/2 store 1, anything else stores 0.
static int __cdecl GfxLights_UpdateProviderFlagFromState(C_0085e7b8* a1)
{
    uint8_t modeFlag = 0;
    int state = ((int(__fastcall*)(void*, void*))CSF_AT(void**, a1, 0)[0x1C / 4])(a1, nullptr);

    switch (state)
    {
    case 0:
    case 3:
        modeFlag = 3;
        break;
    case 1:
    case 2:
        modeFlag = 1;
        break;
    default:
        modeFlag = 0;
        break;
    }

    void* provider = GetCachedObjectProvider(a1, nullptr);
    CSF_AT(uint8_t, provider, 2) = modeFlag;
    void* result = GetCachedObjectProvider(a1, nullptr);
    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x006D0EB0, GfxLights_UpdateProviderFlagFromState, "auto");
