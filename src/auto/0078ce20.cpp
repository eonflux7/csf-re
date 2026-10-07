#include <cstdint>

#include "globals.h"
#include "hook.h"

// Vtable pointer/value located at 0x872CA4; its address is what gets stored at offset 0.
CSF_GLOBAL(uint32_t, g_vtbl_872CA4, 0x872CA4);

// Writes the vtable pointer to offset 0 of the object and returns the object (this in eax).
static uint32_t* __fastcall SetVtable_872CA4(uint32_t* self, void* /*edx*/)
{
    *self = reinterpret_cast<uint32_t>(&g_vtbl_872CA4);
    return self;
}

HOOK(0x78CE20, SetVtable_872CA4, "auto");
