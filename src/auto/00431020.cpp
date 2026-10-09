#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
// sub_42F4F0 is __cdecl-style here: the caller pushes two args and does `add esp, 8`.
// Its first input, edi, is a register input that plain C++ cannot read (see notes).
CSF_FUNC(void, __cdecl, Hud_DrawWidgetFromEdi, 0x0042F4F0, (int* widgetHandle, int a3));

// Hook target: 0x00431020. No stack args, ret 0. Self arrives in ecx, edx unused.
static void __fastcall Hud_DrawSceneMenuWidget(void* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x11A) == 0)
        return;

    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    uint32_t menuMode = 0;
    if (sceneMenuContext != 0)
        menuMode = CSF_AT(uint32_t, sceneMenuContext, 0x1C4);

    // Pointer P = [self+0x24] is loaded once per branch, as in the original.
    void* widgetListPtr = CSF_AT(void*, self, 0x24);

    if (menuMode == 0) {
        // push [P]
        void* first = *reinterpret_cast<void**>(widgetListPtr);
        Hud_DrawWidgetFromEdi(reinterpret_cast<int*>(first), 0);
    } else if (menuMode == 1) {
        // push [P+4]
        int* second = CSF_AT(int*, widgetListPtr, 4);
        Hud_DrawWidgetFromEdi(second, 0);
    }
}

HOOK(0x00431020, Hud_DrawSceneMenuWidget, "auto");
