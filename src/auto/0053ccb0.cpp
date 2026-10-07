#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x0053CCB0 (15 bytes, __thiscall, ret 0)
// Forwards to the virtual method at vtable offset +0x2C of the object stored at
// this+0x2C, passing the value at this+0x38 as its single argument.
static int __fastcall CMenuSoundOptions_CallMemberVirtualSlot11(CMenuSoundOptions* self, void* /*edx*/)
{
    void* member = CSF_AT(void*, self, 0x2c);
    uint32_t arg = CSF_AT(uint32_t, self, 0x38);
    void** vtbl = CSF_AT(void**, member, 0);
    return ((int(__fastcall*)(void*, void*, uint32_t))vtbl[0x2c / 4])(member, nullptr, arg);
}

HOOK(0x0053CCB0, CMenuSoundOptions_CallMemberVirtualSlot11, "auto");
