#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));

// Looks up the frame provider object for this object's stored clump handle,
// then returns the attachment index for that provider object.
static int __fastcall C_00862900_GetObjectAttachmentOfProviderFrame(C_00862900* self, void* /*edx*/) {
    int providerFrameObject = GfxFrames_GetProviderFrameObject(static_cast<int>(self->field_0004));
    return GfxObjectProps_GetObjectAttachment(providerFrameObject);
}

HOOK(0x006F7370, C_00862900_GetObjectAttachmentOfProviderFrame, "auto");
