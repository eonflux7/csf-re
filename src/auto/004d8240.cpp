#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082f9f8_StartImpactAnim, 0x004208E0, (C_0082f9f8* self, void* /*edx*/, float scale));

// Sets bit 0 in the flags word of the owner object (self+8), then starts the
// impact animation on the object at owner->field_0014->field_0030.
static char __fastcall Autonomo_SetImpactFlagAndStartAnim(void* self, void* /*edx*/, float scale)
{
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, owner, 0x18) |= 1u;

    void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    C_0082f9f8* anim = reinterpret_cast<C_0082f9f8*>(CSF_AT(uint32_t, inner, 0x30));
    return C_0082f9f8_StartImpactAnim(anim, nullptr, scale);
}

HOOK(0x004D8240, Autonomo_SetImpactFlagAndStartAnim, "auto");
