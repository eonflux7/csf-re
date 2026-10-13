#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwEngineGetVideoModeInfo, 0x00626BE0, (int a1, int a2));

// Forwards to RwEngineGetVideoModeInfo with its two arguments swapped
// (the original pushes a1 first, so the callee receives a2 as its first argument).
static int __stdcall CRenderEngine_GetVideoModeInfoSwapped(int a1, int a2)
{
    return RwEngineGetVideoModeInfo(a2, a1);
}

HOOK(0x6DBA50, CRenderEngine_GetVideoModeInfoSwapped, "auto");
