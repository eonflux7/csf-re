#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __fastcall, C_0083e500_SetField0008, 0x00713B10, (C_0083e500* self, void* , int value));

// Removes the entry whose key matches the object attachment of the given handle from the
// pointer array at +0x1C (count at +0x24). The removed entry is released via its vtable slot 1,
// the last element is moved into the freed slot, and the attachment's field 0x0008 is cleared.
static unsigned int __fastcall GfxSkin_RemoveAttachmentEntry(void* self, void* /*edx*/, int attachmentHandle)
{
    C_0083e500* objectAttachment =
        reinterpret_cast<C_0083e500*>(SceneLoader_GetObjectAttachment(attachmentHandle));
    uint32_t result = CSF_AT(uint32_t, self, 0x24);
    uint32_t foundIndex = 0;

    if (result != 0)
    {
        // Linear search for the attachment key among the stored entries.
        foundIndex = 0;
        while (reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C))[foundIndex] != objectAttachment->field_0008)
        {
            ++foundIndex;
            if (foundIndex >= result)
                return result;
        }

        if (foundIndex != 0xFFFFFFFFu)
        {
            // Release the entry stored at the found slot, if any.
            void* entry = reinterpret_cast<void*>(
                reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C))[foundIndex]);
            if (entry != nullptr)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, entry, 0)[1])(entry, nullptr, 1);
                reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C))[foundIndex] = 0;
            }

            // Move the last element into the freed slot and shrink the count.
            uint32_t lastIndex = CSF_AT(uint32_t, self, 0x24) - 1;
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C))[foundIndex] =
                reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x1C))[lastIndex];
            CSF_AT(uint32_t, self, 0x24) = lastIndex;

            return static_cast<unsigned int>(C_0083e500_SetField0008(objectAttachment, nullptr, 0));
        }
    }
    return result;
}

HOOK(0x006C5C10, GfxSkin_RemoveAttachmentEntry, "auto");
