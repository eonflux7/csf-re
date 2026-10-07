#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x004D81C0 (size 15 bytes, __thiscall, ret 0 -> no stack arguments)
//
//   mov eax, [ecx+8]        ; eax = *(uint32_t*)(this + 8)  (a pointer)
//   mov ecx, [eax+0x18]     ; ecx = flags
//   add eax, 0x18           ; eax = &flags
//   and ecx, 0xFFFFFFFD     ; clear bit 1
//   mov [eax], ecx          ; *flags = ecx
//   ret                     ; return &flags
static uint32_t* __fastcall ClearInnerObjectFlagBit1(void* self, void* /*edx*/)
{
    // Inner object pointer stored at this + 8.
    uint8_t* inner = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 8)));

    // Flags word at inner + 0x18: clear bit 1, keep everything else.
    uint32_t* flags = reinterpret_cast<uint32_t*>(inner + 0x18);
    *flags &= 0xFFFFFFFDu;

    return flags;
}

HOOK(0x4D81C0, ClearInnerObjectFlagBit1, "auto");
