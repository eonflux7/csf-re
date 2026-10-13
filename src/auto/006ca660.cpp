#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (C_00860894* self, void*, int a2, int a3));

// Initializes the C_00860a30 sub-object: builds its vtable chain, sets the
// default flags, and allocates a 28-byte helper object that points back here.
static int __fastcall C_00860a30_InitBase(C_00860a30* self, void* /*edx*/)
{
    C_00860178_Construct(self, nullptr);

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D428));
    self->vtbl = CSF_VTBL(0x00860A30);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00860A24));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00860A08));
    self->field_0014 = 0;
    self->field_000c = 1;
    self->field_0014 |= 2u;
    self->field_0010 = static_cast<uint32_t>(-1);
    self->field_0018 = 0;

    int allocatedValue = CoreMemory_AllocUntagged(28);
    if (allocatedValue != 0)
    {
        void* allocated = reinterpret_cast<void*>(allocatedValue);
        C_00860178_Construct(reinterpret_cast<C_00860178*>(allocated), nullptr);
        CSF_AT(void*, allocated, 0) = CSF_VTBL(0x00860AB0);
        C_00860894_Init(reinterpret_cast<C_00860894*>(static_cast<uint8_t*>(allocated) + 8), nullptr, -1, 0);
        CSF_AT(uint32_t, allocated, 4) = reinterpret_cast<uint32_t>(self);
        self->field_001c = static_cast<uint32_t>(allocatedValue);
    }
    else
    {
        self->field_001c = 0;
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x006CA660, C_00860a30_InitBase, "auto");
