#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(uint32_t, __fastcall, GfxObjectProps_SetFlagBit01, 0x006B8B80, (void* self, void* , char enable));

// Walks the attachment table of a gfx object (count at +0x1C, entry pointers at +0x18)
// and applies the flag-bit setter to each entry's attachment. Returns the last setter result.
static int __fastcall GfxObject_SetFlagBitOnAllAttachments(void* self, void* /*edx*/, char enable)
{
    int result = 0;
    for (unsigned int i = 0; i < CSF_AT(uint16_t, self, 0x1C); ++i)
    {
        void* entry = CSF_AT(void**, self, 0x18)[i];
        int attachment = CSF_AT(int, entry, 0x0C);
        void* attachmentObject = (void*)(uintptr_t)GfxObjectProps_GetObjectAttachment(attachment);
        result = (int)GfxObjectProps_SetFlagBit01(attachmentObject, nullptr, enable);
    }
    return result;
}

HOOK(0x006B2F50, GfxObject_SetFlagBitOnAllAttachments, "auto");
