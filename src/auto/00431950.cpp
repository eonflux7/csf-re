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

// Resolves a HUD sub-object for the given request id via 0x42EBA0, then forwards it
// to 0x42F4F0 (render at its own rect) when the lookup succeeds.
CSF_FUNC(int, __fastcall, ResolveHudSubObject, 0x0042EBA0, (void* self, void* edx, int a2));
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

static void __fastcall CHud_DispatchToResolvedControl(void* self, void* /*edx*/, int requestId)
{
    int* resolvedSubObject = reinterpret_cast<int*>(
        static_cast<intptr_t>(static_cast<int32_t>(
            ResolveHudSubObject(self, nullptr, requestId))));

    if (resolvedSubObject) {
        Hud_RenderWidgetAtScaledRect(resolvedSubObject, nullptr);
    }
}

HOOK(0x00431950, CHud_DispatchToResolvedControl, "auto");
