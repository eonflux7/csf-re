#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, C_Unk41E3D0_GetField14Plus5C, 0x0041E3D0, (void* self, void*));

// __thiscall int AiTaskState::SnapshotFieldAndDispatch(void) : copies a 12-byte
// snapshot from owner->+0x29C into owner->+0x218, clears this->+0x14, then
// dispatches vtable slot 0x8C of owner->+0x14 with (owner->+0x238, 0, -1, 0).
static int __fastcall AiTaskState_SnapshotFieldAndDispatch(void* self, void* /*edx*/)
{
    uint8_t* owner = (uint8_t*)CSF_AT(void*, self, 8);

    // Source snapshot is returned by the getter (called with owner+0x29C as this).
    uint32_t* src = (uint32_t*)C_Unk41E3D0_GetField14Plus5C(owner + 0x29C, nullptr);

    // Copy 3 dwords into owner+0x218.
    CSF_AT(uint32_t, owner, 0x218) = src[0];
    CSF_AT(uint32_t, owner, 0x21C) = src[1];
    CSF_AT(uint32_t, owner, 0x220) = src[2];

    CSF_AT(uint8_t, self, 0x14) = 0;

    // owner->+0x14 is a sub-object with a vtable; slot 0x8C / 4 = 35.
    void* target = CSF_AT(void*, owner, 0x14);
    int argument = (int)CSF_AT(uint32_t, owner, 0x238);

    return ((int(__fastcall*)(void*, void*, int, int, int, int))
                CSF_AT(void**, target, 0)[0x8C / 4])(target, nullptr, argument, 0, -1, 0);
}

HOOK(0x004210D0, AiTaskState_SnapshotFieldAndDispatch, "auto");
