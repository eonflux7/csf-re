#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_485010, 0x00485010, (void* self, void* edx, int a2));

static int __fastcall CCharWeaponFsm_RequestWeaponAction(CCharWeaponFsm* self, void* /*edx*/)
{
    // 0x28-byte stack object passed by address to sub_485010 (ecx = object, arg = &field_003c).
    alignas(4) uint8_t callbackBuf[0x28] = {};
    sub_485010(callbackBuf, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&self->field_003c)));

    // Byte at +0xA8 is read before the object is filled and stored into the object.
    uint8_t flagByte = CSF_AT(uint8_t, self, 0xA8);

    uint32_t* words = reinterpret_cast<uint32_t*>(callbackBuf);
    words[0] = 0;                   // ebp-0x28
    words[2] = 0xFFFFFFFFu;         // ebp-0x20
    words[3] = 0xFFFFFFFFu;         // ebp-0x1C
    callbackBuf[0x20] = 0;          // ebp-0x08
    callbackBuf[0x25] = flagByte;   // ebp-0x03
    callbackBuf[0x23] = 1;          // ebp-0x05

    // Object at +0xA4 is called through vtable slot 0x40 / 4 = 16 (__thiscall, one stack arg).
    void* stateMachine = CSF_AT(void*, self, 0xA4);
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, stateMachine, 0)[0x40 / 4])(
        stateMachine, nullptr, callbackBuf);

    // Flag is re-read after the call: 55 when set, 22 otherwise.
    return self->vSlot11C(CSF_AT(uint8_t, self, 0xA8) != 0 ? 55 : 22);
}

HOOK(0x00485190, CCharWeaponFsm_RequestWeaponAction, "auto");
