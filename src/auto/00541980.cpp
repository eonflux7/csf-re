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

// Base sub-object initializer at 0x712CC0 (__thiscall, no stack args).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

// Constructs CMenuMpStats in place. Takes one stack argument (popped by ret 4).
// The original stores vtable addresses as immediates, so they are written as immediates here.
// Store order follows the original: primary vtable, base init call, owner arg,
// secondary vtable, zeroed fields, final vtable.
static CMenuMpStats* __fastcall CMenuMpStats_Construct(CMenuMpStats* self, void* /*edx*/, int ownerArg)
{
    // mov dword ptr [esi], 0x83042c
    *reinterpret_cast<uint32_t*>(self) = CSF_ADDR_VT(0x0083042C);

    // call 0x712CC0 on this+4
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    // mov eax, [ebp+8]; mov [esi+0x88], eax
    self->field_0088 = static_cast<uint32_t>(ownerArg);

    // mov dword ptr [edi], 0x8431e0   (edi = this+4)
    *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 4) = CSF_ADDR_VT(0x008431E0);

    // Zero the stats fields.
    self->field_0084 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_004e = 0;
    self->field_0080 = 0;

    // mov dword ptr [esi], 0x843230
    *reinterpret_cast<uint32_t*>(self) = CSF_ADDR_VT(0x00843230);

    return self;
}

HOOK(0x00541980, CMenuMpStats_Construct, "auto");
