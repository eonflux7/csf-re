#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(bool, __fastcall, SceneCheckPairLink, 0x004687E0, (void* self, void* edx, int a2, int a3));

// Tests a pair of nodes taken from the caller's argument block against the current
// scene's link checker, then stores the boolean result as a byte at (a1->field_04 + 1).
static int __stdcall EvaluateSceneLinkAndStoreFlag(void* a1, int /*a2*/, uint32_t* a3)
{
    uint8_t* flagTarget = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, a1, 4));

    void* scene = g_pCurrentScene;
    void* sceneChecker = CSF_AT(void*, scene, 0xBC);

    void* firstNode = reinterpret_cast<void*>(a3[0]);
    int firstArg = static_cast<int>(CSF_AT(uint32_t, firstNode, 4) + 4);

    void* secondNode = reinterpret_cast<void*>(a3[1]);
    void* secondInner = reinterpret_cast<void*>(CSF_AT(uint32_t, secondNode, 4));
    int secondArg = static_cast<int>(CSF_AT(uint32_t, secondInner, 4));

    bool result = SceneCheckPairLink(sceneChecker, nullptr, firstArg, secondArg);
    CSF_AT(uint8_t, flagTarget, 1) = static_cast<uint8_t>(result);
    return static_cast<int>(result);
}

HOOK(0x0045AF90, EvaluateSceneLinkAndStoreFlag, "auto");
