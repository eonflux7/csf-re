#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bound by address: the tail target at 0x48C9B0. Its exact role is unknown, so it gets a neutral name.
CSF_FUNC(int, __fastcall, DispatchOwnerSubobject, 0x0048C9B0, (void* self, void* edx));

// Clears bit 1 (0x2) in the flags word at +0x18 of the object referenced by this+0x08, then
// forwards to 0x48C9B0 with ecx = owner->+0x14->+0x204 and edx unchanged (tail jump).
static int __fastcall AutonomoClearFlagAndDispatch(void* self, void* edx)
{
    uint8_t* obj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    CSF_AT(uint32_t, obj, 0x18) &= ~2u;

    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, obj, 0x14));
    void* dispatchArg = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x204));
    return DispatchOwnerSubobject(dispatchArg, edx);
}

HOOK(0x004D7D00, AutonomoClearFlagAndDispatch, "auto");
