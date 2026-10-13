#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: GfxObjectProps_GetObjectAttachment(int objectIndex), __cdecl, ret 0.
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));

// Original: SceneLoader_SetAttachmentFlagBit20, __thiscall-style, ret 4.
CSF_FUNC(uint32_t, __fastcall, SceneLoader_SetAttachmentFlagBit20, 0x006B8C10, (void* self, void* , char enable));

// Copies a "non-zero" byte flag from *source into bit 0 of the object's byte at +2,
// then forwards the result to the attachment via SceneLoader.
static int __cdecl GfxObjectProps_SyncAttachmentFlagFromSource(int objectHandle, char* sourceFlag)
{
    uint8_t* object = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(objectHandle));

    char sourceValue = *sourceFlag;
    uint8_t flagByte = CSF_AT(uint8_t, object, 2);
    if (sourceValue != 0)
        flagByte = static_cast<uint8_t>(flagByte | 1);
    else
        flagByte = static_cast<uint8_t>(flagByte & 0xFE);
    CSF_AT(uint8_t, object, 2) = flagByte;

    char enableAttachmentBit = (sourceValue == 0) ? 1 : 0;

    void* attachment = reinterpret_cast<void*>(static_cast<uintptr_t>(
        GfxObjectProps_GetObjectAttachment(objectHandle)));
    SceneLoader_SetAttachmentFlagBit20(attachment, nullptr, enableAttachmentBit);

    return objectHandle;
}

HOOK(0x006F7230, GfxObjectProps_SyncAttachmentFlagFromSource, "auto");
