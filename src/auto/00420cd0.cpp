#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bind as given by the packet (one name and signature per address).
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

// Original: 0x00420CD0, __thiscall with two stack args, ret 8.
// Behavior:
//   1. owner = this->field_0008; child = owner->field_0014; child's sub-object at +4
//      gets its vtable slot 4 called (no arguments).
//   2. owner->field_0018 |= 4 (owner re-read after the call).
//   3. return sub_4DB440(this, this->field_000C + 0x20).
static int __fastcall C_00834fa8_SetOwnerFlag4AndAttach(C_00834fa8* self, void* /*edx*/, int /*a2*/, int /*a3*/) {
    // Notify the owner's child object through its vtable (slot 16 / 4 = 4).
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* childSubObject = CSF_AT(uint8_t*, owner, 0x14) + 4;
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, childSubObject, 0)[4])(childSubObject, nullptr);

    // Set flag bit 4 on the owner (owner pointer re-read after the virtual call).
    uint8_t* ownerAfterCall = CSF_AT(uint8_t*, self, 0x08);
    CSF_AT(uint32_t, ownerAfterCall, 0x18) |= 4u;

    // Attach this object using the sub-object at field_000C + 0x20.
    int attachAddress = CSF_AT(int, self, 0x0C) + 0x20;
    return sub_4DB440(self, nullptr, attachAddress);
}

HOOK(0x420CD0, C_00834fa8_SetOwnerFlag4AndAttach, "auto");
