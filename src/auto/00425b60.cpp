#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall: computes base + index * 20, where base is the dword at +0x14 and
// index is the dword at +0x24 (stride-20 element lookup into an array).
static int __fastcall C_Unknown_ComputeIndexedElement(void* self, void* /*edx*/)
{
    const uint32_t index = CSF_AT(uint32_t, self, 0x24);
    const uint32_t base  = CSF_AT(uint32_t, self, 0x14);

    // lea eax,[eax+eax*4] -> index*5 ; lea eax,[ecx+eax*4] -> base + index*20
    return static_cast<int>(base + 20u * index);
}

HOOK(0x425B60, C_Unknown_ComputeIndexedElement, "auto");
