#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Reads the 16-bit word stored at offset 0xC of the object referenced by the
// pointer held at offset 0x4 of `self`.
//   mov eax, [ecx+4]         ; inner object
//   movzx eax, word [eax+0xC]
static uint16_t __fastcall GetInnerObjectWord12(void* self, void* /*edx*/)
{
    void* innerObject = CSF_AT(void*, self, 0x4);
    return CSF_AT(uint16_t, innerObject, 0xC);
}

HOOK(0x6FF550, GetInnerObjectWord12, "auto");
