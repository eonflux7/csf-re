#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneMenuContext_HandleSound, 0x00525A50, (void* self, void* edx, int a3, int a4));

static char __fastcall SoundManager_PlaySoundInSceneMenuContext(int* self, void* /*edx*/, int soundArg)
{
    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (sceneMenuContext)
        return SceneMenuContext_HandleSound(self, nullptr, (int)sceneMenuContext, soundArg);
    return 0;
}

HOOK(0x525AB0, SoundManager_PlaySoundInSceneMenuContext, "auto");
