#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x0069AFD0 (14 bytes)
//   mov eax, ecx
//   mov byte ptr [eax], 0
//   mov byte ptr [eax + 0x28], 1
//   mov byte ptr [eax + 0x29], 0
//   ret
// __thiscall initializer: clears the object's first byte, sets the
// flag byte at +0x28 to 1 and the byte at +0x29 to 0, then returns `this`.
static uint8_t* __fastcall InitObjectState(uint8_t* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(uint8_t, self, 0x28) = 1;
    CSF_AT(uint8_t, self, 0x29) = 0;
    return self;
}

HOOK(0x69AFD0, InitObjectState, "auto");
