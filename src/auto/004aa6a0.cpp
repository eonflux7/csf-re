#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Scene_CanTraverseSegment, 0x004DF2E0, (void* self, void* edx, unsigned int segmentAddress));

// Asks the current scene's GUI control chain whether the given segment can be traversed.
static int __stdcall AiCore_TestQuerySegment(int a1)
{
    uint32_t field012C = CGuiControl_GetField012C(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);

    int itemCount = GetCurrentItemCount(reinterpret_cast<void*>(field012C), nullptr);

    return Scene_CanTraverseSegment(
        reinterpret_cast<void*>(static_cast<intptr_t>(itemCount)),
        nullptr,
        static_cast<unsigned int>(a1));
}

HOOK(0x4AA6A0, AiCore_TestQuerySegment, "auto");
