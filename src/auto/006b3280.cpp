#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bound original callees (addresses from the packet).
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, GfxEnvironment_GetAttachmentId, 0x006B8080, (void* self, void* edx));

// Checks whether the attachment id of objectIndex matches the request record at requestRecord.
// The record holds: [0] expected attachment id, [4] result slot, [8] flag allowing an unassigned (-1) match.
// Returns objectIndex when it does not match (caller keeps searching); otherwise stores the
// index in the record's result slot and returns 0.
static int __cdecl GfxObject_FindAttachmentMatch(int objectIndex, int requestRecord)
{
    void* objectAttachment = reinterpret_cast<void*>(GfxObjectProps_GetObjectAttachment(objectIndex));
    int attachmentId = GfxEnvironment_GetAttachmentId(objectAttachment, nullptr);

    void* record = reinterpret_cast<void*>(requestRecord);
    if (attachmentId != CSF_AT(int, record, 0x0) ||
        (attachmentId == -1 && !CSF_AT(uint8_t, record, 0x8)))
    {
        return objectIndex;
    }

    CSF_AT(int, record, 0x4) = objectIndex;
    return 0;
}

HOOK(0x006B3280, GfxObject_FindAttachmentMatch, "auto");
