#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls, bound by address.
CSF_FUNC(int, __cdecl, GfxFrames_GetProviderFrameObject, 0x006C0500, (int clump));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(void*, __fastcall, FxParticleManager_GetAttachmentTransform, 0x006B8B10, (void* self, void* /*edx*/, void* outBuffer));

// __thiscall with one stack argument (ret 4): ecx = this, edx unused, stack = output buffer.
// Fills outBuffer with the 60-byte attachment transform of the object's provider frame and returns outBuffer.
static void* __fastcall C_00862900_CopyAttachmentTransform(C_00862900* self, void* /*edx*/, void* outBuffer)
{
    alignas(8) uint8_t transformBytes[0x3C];

    int providerFrameObject = GfxFrames_GetProviderFrameObject(static_cast<int>(self->field_0004));
    int objectAttachment = GfxObjectProps_GetObjectAttachment(providerFrameObject);
    FxParticleManager_GetAttachmentTransform(reinterpret_cast<void*>(objectAttachment), nullptr, transformBytes);

    std::memcpy(outBuffer, transformBytes, 0x3C);
    return outBuffer;
}

HOOK(0x006F73F0, C_00862900_CopyAttachmentTransform, "auto");
