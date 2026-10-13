#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, GetUint16OrSentinelMinusOne, 0x006B80A0, (void* self, void*));

// Looks up the provider frame object for this object's stored clump, takes its
// object attachment, and returns the attachment's uint16 value (or -1 sentinel).
static int __fastcall C_00862900_GetProviderAttachmentUint16(C_00862900* self, void* /*edx*/) {
    int providerFrameObject = GfxFrames_GetProviderFrameObject(static_cast<int>(self->field_0004));
    int objectAttachment = GfxObjectProps_GetObjectAttachment(providerFrameObject);
    return GetUint16OrSentinelMinusOne(reinterpret_cast<void*>(objectAttachment), nullptr);
}

HOOK(0x006F7390, C_00862900_GetProviderAttachmentUint16, "auto");
