#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxEnvironment_EntryToObjectIndex, 0x006BCE10, (int entryValue));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, GfxEnvironment_GetAttachmentId, 0x006B8080, (void* self, void* edx));

// Walks the environment's entry table and returns the first entry whose
// object attachment id equals attachmentId. Returns 0 if none matches.
static int __fastcall C_008605d8_FindEntryByAttachmentId(C_008605d8* self, void* /*edx*/, int attachmentId)
{
    if (self->field_0014 == 0)
        return 0;

    uint32_t index = 0;
    for (;;)
    {
        int entryValue = *reinterpret_cast<int*>(self->field_000c + 4u * index);
        int objectIndex = GfxEnvironment_EntryToObjectIndex(entryValue);
        int attachment = GfxObjectProps_GetObjectAttachment(objectIndex);
        if (GfxEnvironment_GetAttachmentId(reinterpret_cast<void*>(attachment), nullptr) == attachmentId)
            return entryValue;

        ++index;
        if (index >= self->field_0014)
            return 0;
    }
}

HOOK(0x006BD260, C_008605d8_FindEntryByAttachmentId, "auto");
