#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

// Interfaz INSTANCING SCENE/RESULT: if the current scene's sub-object (at +0xC0) has
// its flag byte (+9) clear, stores the requested flag at this+0x183 and returns it;
// otherwise returns the scene's flag byte.
static char __fastcall Interface_SetFlagIfSceneReady(uint8_t* self, void* /*edx*/, char flag) {
    uint8_t* sceneSub = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, g_pCurrentScene, 0xC0));
    char sceneFlag = static_cast<char>(CSF_AT(uint8_t, sceneSub, 9));
    if (!sceneFlag) {
        CSF_AT(char, self, 0x183) = flag;
        return flag;
    }
    return sceneFlag;
}

HOOK(0x5365D0, Interface_SetFlagIfSceneReady, "auto");
