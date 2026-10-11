#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the (vtable, value1, value2) triple at element `index` of the table stored at self+0x0C.
// Table elements are 12 bytes each; the first dword of an element is not read.
// The result is written into the caller-supplied `out` object, whose first dword is the
// C_0082d448 vtable pointer.
static uint32_t* __fastcall GetEntryAtIndex(void* self, void* /*edx*/, void* out, int index)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0xC)) + index * 12;

    *(void***)out = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, out, 4) = CSF_AT(uint32_t, entry, 4);
    CSF_AT(uint32_t, out, 8) = CSF_AT(uint32_t, entry, 8);
    return reinterpret_cast<uint32_t*>(out);
}

HOOK(0x004E5B10, GetEntryAtIndex, "auto");
