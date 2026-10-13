#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Subtitle panel positioning routine, forwarded as a tail jump.
CSF_FUNC(int, __fastcall, SetSubtitlePanelPosition, 0x004F1CE0, (void* self, void* edx, float x, float y));

// Reads the panel object stored at this+4 and forwards the position
// arguments to SetSubtitlePanelPosition (tail call, stack args unchanged).
static int __fastcall HudStringShow_ForwardSetSubtitlePanelPosition(CCamera** self, void* /*edx*/, float x, float y)
{
    void* subtitlePanel = CSF_AT(void*, self, 0x4);
    return SetSubtitlePanelPosition(subtitlePanel, nullptr, x, y);
}

HOOK(0x4F4970, HudStringShow_ForwardSetSubtitlePanelPosition, "auto");
