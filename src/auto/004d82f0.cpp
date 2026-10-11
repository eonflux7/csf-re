#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_4A7240, 0x004A7240, (void* self, void* edx));

// Sets bit 0 of the flags dword at +0x18 of the owner (self->+8), then tail-jumps
// into 0x4A7240 with the owner's +0x310 pointer as `this`.
static int __fastcall TAutonomo_MarkFlag1AndForwardTo310(void* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, owner, 0x18) |= 1u;
    C_0082f9f8* target = reinterpret_cast<C_0082f9f8*>(CSF_AT(uint32_t, owner, 0x310));
    return sub_4A7240(target, nullptr);
}

HOOK(0x4D82F0, TAutonomo_MarkFlag1AndForwardTo310, "auto");
