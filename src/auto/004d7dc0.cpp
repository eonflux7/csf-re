#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_492C10, 0x00492C10, (void* self, void* edx, int a1, int a2));

// __thiscall method: ecx = this (uint32_t* self), 4 stack args (ret 0x10).
static int __fastcall Autonomo_SetFlagAndForward(uint32_t* self, void* /*edx*/, int a2, int a3, char a4, int a5)
{
    // Stage 1: sub_492C10(this->field_08->field_14, 1, a5)
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* ownerChild = CSF_AT(uint8_t*, owner, 0x14);
    sub_492C10(ownerChild, nullptr, 1, a5);

    // Stage 2: set bit 0 of owner->field_18
    owner = CSF_AT(uint8_t*, self, 0x08);
    CSF_AT(uint32_t, owner, 0x18) |= 1u;

    // Stage 3: store (a4 != 0) into the object at child->field_30, offset 0x1c,
    // then virtual dispatch slot 43 (0xAC) on that object with (a2, a3).
    uint8_t* chain = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* chainChild = CSF_AT(uint8_t*, chain, 0x14);
    uint8_t* target = CSF_AT(uint8_t*, chainChild, 0x30);
    CSF_AT(int, target, 0x1C) = (a4 != 0) ? 1 : 0;

    int result = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, target, 0)[0xAC / 4])(
        target, nullptr, a2, a3);
    return result;
}

HOOK(0x004D7DC0, Autonomo_SetFlagAndForward, "auto");
