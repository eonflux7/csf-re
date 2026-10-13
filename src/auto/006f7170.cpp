#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(uint32_t, __fastcall, SetFlagBit08, 0x006B8C40, (void* self, void* , char enable));

static int __cdecl GfxObject_SetAttachmentFlagBit08IfAttached(int objectIndex)
{
    int attachment = GfxObjectProps_GetObjectAttachment(objectIndex);
    if (attachment != 0)
    {
        SetFlagBit08(reinterpret_cast<void*>(attachment), nullptr, 1);
    }
    return objectIndex;
}

HOOK(0x006F7170, GfxObject_SetAttachmentFlagBit08IfAttached, "auto");
