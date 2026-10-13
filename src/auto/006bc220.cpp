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

// Bound original callees (by address, signatures as given in the packet).
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, AttachmentIsComArray, 0x006B8130, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, C_008656d0_ApplyEntryParameters, 0x006C63D0, (C_008656d0* self, void* edx, int entryAddress));

// Updates the attachment's cached value at +0x0C from props+8; if the attachment is a
// COM array with a non-zero entry at +8, applies that entry through the global value.
static int __cdecl GfxEnvironment_UpdateAttachmentFromProps(int objectIndex, int props)
{
    uint8_t* attachment = reinterpret_cast<uint8_t*>(GfxObjectProps_GetObjectAttachment(objectIndex));
    int newValue = CSF_AT(int, reinterpret_cast<uint8_t*>(props), 8);

    if (CSF_AT(int, attachment, 0xC) != newValue)
    {
        CSF_AT(int, attachment, 0xC) = newValue;
        if (AttachmentIsComArray(attachment, nullptr) & 0xFF)
        {
            int entryAddress = CSF_AT(int, attachment, 8);
            if (entryAddress != 0)
            {
                C_008656d0* globalValue = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
                C_008656d0_ApplyEntryParameters(globalValue, nullptr, entryAddress);
            }
        }
    }
    return objectIndex;
}

HOOK(0x006BC220, GfxEnvironment_UpdateAttachmentFromProps, "auto");
