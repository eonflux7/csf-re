#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsBit5Set, 0x006B8190, (void* self, void*));
CSF_FUNC(int, __fastcall, GetInnerValueAtOffset4, 0x006B81D0, (void* self, void*));

static bool __cdecl GfxCollision_IsAttachmentEligible(C_0085ec18* attachment)
{
    if (static_cast<uint8_t>(IsBit5Set(attachment, nullptr)))
        return true;

    if (static_cast<uint8_t>(ObjectAttachment_HasSoundTrigger(attachment, nullptr)))
        return false;

    const int innerValue = GetInnerValueAtOffset4(attachment, nullptr);
    if (innerValue &&
        CSF_AT(uint32_t, reinterpret_cast<void*>(innerValue), 0x14))
        return false;

    return true;
}

HOOK(0x006EBE50, GfxCollision_IsAttachmentEligible, "auto");
