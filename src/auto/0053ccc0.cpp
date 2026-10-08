#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Forwards the byte flag at +0x3C to a virtual method (vtable slot 0x34/4 = 13)
// of the member object stored at +0x2C, passing (flag, 0).
static int __fastcall CMenuSoundOptions_DispatchFlagToMember(CMenuSoundOptions* self, void* /*edx*/)
{
    void* member = reinterpret_cast<void*>(self->field_002c);
    void** vtbl = *reinterpret_cast<void***>(member);
    return (reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t, uint32_t)>(vtbl[13]))(
        member, nullptr, self->field_003c, 0);
}

HOOK(0x0053CCC0, CMenuSoundOptions_DispatchFlagToMember, "auto");
