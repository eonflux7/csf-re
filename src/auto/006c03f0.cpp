#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bound callees (addresses from the packet).
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(bool, __fastcall, GfxObjectProps_HasNonZeroOffsetXY, 0x006B8B40, (void* self, void* edx));

// Returns the object index unchanged when it has no attachment or the attachment
// has no non-zero XY offset. Otherwise sets *flagOut = 1 and returns 0.
static int __cdecl GfxFrames_FlagIfObjectHasOffset(int objectIndex, uint8_t* flagOut)
{
    int attachment = GfxObjectProps_GetObjectAttachment(objectIndex);
    if (attachment == 0 || !GfxObjectProps_HasNonZeroOffsetXY(reinterpret_cast<void*>(attachment), nullptr))
        return objectIndex;

    *flagOut = 1;
    return 0;
}

HOOK(0x006C03F0, GfxFrames_FlagIfObjectHasOffset, "auto");
