// Commandos: Strike Force – reimplementation of 0x00726FF0 (game region).
//
// Indexes into an array (stride 36 bytes) of string-like objects pointed to by
// this->field_0048 and returns the address of the character data of the
// requested element.  Each element stores its data either inline at +8 (small
// string case, capacity field at +0x1c < 16) or as a heap pointer at +8.

#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall: (this, int index) -> char data pointer; ret 4
static void* __fastcall C_008655e8_GetStringData(C_008655e8* self, void* /*edx*/, int index)
{
    // element = array_base + index * 36    (lea eax,[eax+eax*8]; lea eax,[ecx+eax*4])
    uint8_t* element = reinterpret_cast<uint8_t*>(self->field_0048) + index * 36;

    // jb (unsigned) on the capacity field at +0x1c: below 16 => inline buffer
    if (CSF_AT(uint32_t, element, 0x1c) < 0x10u)
        return element + 8;

    return reinterpret_cast<void*>(CSF_AT(uint32_t, element, 0x8));
}

HOOK(0x726FF0, C_008655e8_GetStringData, "auto");
