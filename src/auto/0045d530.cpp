#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, TryAcceptSceneTarget, 0x004448D0, (void* self, void* edx, void* a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall with three stack args (a2, a3, a4) -> ret 12.
static char __fastcall CaptureEventTargetAndAdvance(void* self, void* /*edx*/, void* /*a2*/, void* /*a3*/, void* a4)
{
    // Walk the event reference chain: target = (*(*a4 + 4))->field_4
    uint8_t* eventObject = *static_cast<uint8_t**>(a4);
    uint8_t* eventLink = *reinterpret_cast<uint8_t**>(eventObject + 4);
    void* target = *reinterpret_cast<void**>(eventLink + 4);

    CSF_AT(void*, self, 0x74) = target;

    void* sceneChild = CSF_AT(void*, g_pCurrentScene, 0xC0);
    char result = TryAcceptSceneTarget(sceneChild, nullptr, target);
    if (result)
    {
        CSF_AT(uint32_t, self, 0x18) = 6;
        CSF_AT(uint8_t, self, 0x82) = 0;
    }
    return result;
}

HOOK(0x0045D530, CaptureEventTargetAndAdvance, "auto");
