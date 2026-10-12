#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, AttachmentRecord_Resolve, 0x006B8230, (void* self, void* edx));

// Reads self->propsRef (+4), then propsRef->objectIndex (+0x0C), looks up the object's
// attachment, and tail-jumps into AttachmentRecord_Resolve with the attachment as `this`.
static int __fastcall GfxObjectInstance_ResolveAttachment(void* self, void* /*edx*/)
{
    uint32_t propsRef = CSF_AT(uint32_t, self, 0x4);
    uint32_t objectIndex = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(propsRef)), 0x0C);

    int attachment = GfxObjectProps_GetObjectAttachment(static_cast<int>(objectIndex));

    return AttachmentRecord_Resolve(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(attachment))), nullptr);
}

HOOK(0x006E5F30, GfxObjectInstance_ResolveAttachment, "auto");
