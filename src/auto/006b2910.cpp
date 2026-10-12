#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(uint32_t, __fastcall, SetObjectFlagBit400, 0x006B8BB0, (void* self, void* , char enable));

// Looks up the attachment for the object, clears its flag bit 0x400 on that
// attachment, and returns the original object index unchanged.
static int __cdecl GfxObject_ClearBit400AndReturn(int objectIndex)
{
    int attachment = GfxObjectProps_GetObjectAttachment(objectIndex);
    SetObjectFlagBit400(reinterpret_cast<void*>(static_cast<uintptr_t>(attachment)), nullptr, 0);
    return objectIndex;
}

HOOK(0x006B2910, GfxObject_ClearBit400AndReturn, "auto");
