#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GfxFrames_GetFrameFlagBit2, 0x006C0180, (int frame));

static int __cdecl GfxFrames_ReturnFrameUnlessFlagBit2(int frame, uint8_t* flagOut)
{
    if (!static_cast<uint8_t>(GfxFrames_GetFrameFlagBit2(frame)))
        return frame;
    *flagOut = 1;
    return 0;
}

HOOK(0x006F71D0, GfxFrames_ReturnFrameUnlessFlagBit2, "auto");
