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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, SceneHandler_Dispatch, 0x00450970, (void* self, void* edx, int a2));

// Stdcall, 3 stack args (ret 12). The third argument points to a record whose
// first dword points to a node; that node's +4 points to a sub-node whose +4
// holds the value forwarded to the current scene's handler.
static int __stdcall Scene_ForwardDescriptorToSceneHandler(int /*a1*/, int /*a2*/, int descriptorRef)
{
    uint32_t recordAddr = static_cast<uint32_t>(descriptorRef);
    uint32_t nodeAddr = *reinterpret_cast<uint32_t*>(recordAddr);
    uint32_t subNodeAddr = *reinterpret_cast<uint32_t*>(nodeAddr + 4);
    int forwardedValue = static_cast<int>(*reinterpret_cast<uint32_t*>(subNodeAddr + 4));

    return SceneHandler_Dispatch(g_pCurrentScene, nullptr, forwardedValue);
}

HOOK(0x0045B9F0, Scene_ForwardDescriptorToSceneHandler, "auto");
