#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, SceneLoader_UpdateLoadingState, 0x00521F40, (void* self, void* edx, int a, int b));
CSF_FUNC(unsigned char, __fastcall, AudioSoundManager_FinishLoadContext, 0x005226A0, (void* self, void*));
CSF_FUNC(char, __fastcall, AudioSoundManager_ProcessPendingStops, 0x005227B0, (void* self, void*));

static bool __fastcall MenuManager_SetMenuState(uint8_t* self, void* /*edx*/, char a2)
{
    if (CSF_AT(uint8_t, self, 0x89) != 0)
    {
        if (a2)
        {
            void* loadContext = (void*)(uintptr_t)GetGlobalValue_521BB0();
            SceneLoader_UpdateLoadingState(loadContext, nullptr, 0, 0);
            void* stopContext = (void*)(uintptr_t)GetGlobalValue_521BB0();
            AudioSoundManager_ProcessPendingStops(stopContext, nullptr);
        }
    }
    else if (a2)
    {
        void* loadContext = (void*)(uintptr_t)GetGlobalValue_521BB0();
        SceneLoader_UpdateLoadingState(loadContext, nullptr, 1, 0);
        void* finishContext = (void*)(uintptr_t)GetGlobalValue_521BB0();
        AudioSoundManager_FinishLoadContext(finishContext, nullptr);
    }

    bool result = (CSF_AT(uint8_t, self, 0x89) == 0);
    CSF_AT(uint8_t, self, 0x89) = result ? 1 : 0;
    return result;
}

HOOK(0x0050DC80, MenuManager_SetMenuState, "auto");
