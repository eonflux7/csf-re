#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees (bound by address, signatures copied from the packet).
CSF_FUNC(int*, __fastcall, sub_47EA40, 0x0047EA40, (void* self, void* edx, int a5));
CSF_FUNC(int*, __fastcall, sub_47EC10, 0x0047EC10, (void* self, void* edx, int a5));

// Vtable of the S_0047ea40 sub-object passed to sub_47EA40 / sub_47EC10.
// The original does not store it here; the reference is kept for binding only.
static void* const s_S0047eaVtable = CSF_VTBL(0x0047EA40);

// Copies the state block at a2 into this object, then forwards the two
// embedded sub-objects' data to their copy routines. Returns this.
static int* __fastcall C_00832f18_CopyStateFrom(C_00832f18* self, void* /*edx*/, int a2)
{
    uint8_t* src = reinterpret_cast<uint8_t*>(a2);

    // this->pad_0004 (dword) <- src + 4
    CSF_AT(uint32_t, self, 0x04) = CSF_AT(uint32_t, src, 0x04);

    // S_0047ea40 sub-object at this + 8 <- src + 8
    sub_47EC10(reinterpret_cast<uint8_t*>(self) + 0x08, nullptr, a2 + 8);

    // S_0047ea40 sub-object at this + 0x18 <- src + 0x18
    sub_47EA40(reinterpret_cast<uint8_t*>(self) + 0x18, nullptr, a2 + 24);

    // this->field_0028 <- src + 0x28
    self->field_0028 = CSF_AT(uint32_t, src, 0x28);

    return reinterpret_cast<int*>(self);
}

HOOK(0x0047F310, C_00832f18_CopyStateFrom, "auto");
