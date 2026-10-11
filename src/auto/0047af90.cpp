#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor of TCharTask, bound by address.
CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

// Constructs a C_00831ac0 (TCharTask-derived attack task) in place and returns the object.
static uint32_t* __fastcall C_00831ac0_Construct(C_00831ac0* self, void* /*edx*/, int taskArg) {
    CharTask_InitBase(self, nullptr);
    CSF_AT(int, self, 0x14) = taskArg;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00831AC0);
    CSF_AT(uint32_t, self, 0x04) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00831AA0));
    CSF_AT(int, self, 0x1C) = -1;
    CSF_AT(int, self, 0x20) = -1;
    CSF_AT(void*, self, 0x18) = CSF_VTBL(0x0082D448);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0047AF90, C_00831ac0_Construct, "auto");
