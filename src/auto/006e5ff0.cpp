#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, C_6B81C0_GetFirstItem, 0x006B81C0, (void* self, void*));

// Returns a pointer to the first item's payload inside the object attachment:
// the item's own address + 4 when its +0x18 count is below 16 (inline storage),
// otherwise the pointer stored at +4. Returns 0 when there is no attachment or no first item.
static int __fastcall GfxObjectInstance_GetAttachmentItemPointer(C_00861ce0* self, void* /*edx*/)
{
    int attachment = GfxObjectProps_GetObjectAttachment(static_cast<int>(self->field_000c));
    void* attachmentPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(attachment));

    if (attachment == 0 || C_6B81C0_GetFirstItem(attachmentPtr, nullptr) == 0)
        return 0;

    int firstItem = C_6B81C0_GetFirstItem(attachmentPtr, nullptr);
    uint8_t* firstItemBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(firstItem));

    if (CSF_AT(uint32_t, firstItemBase, 0x18) < 0x10u)
        return firstItem + 4;
    return CSF_AT(int, firstItemBase, 0x4);
}

HOOK(0x006E5FF0, GfxObjectInstance_GetAttachmentItemPointer, "auto");
