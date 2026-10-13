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

// Binds to the original callees. Signatures are copied from the packet; the
// sub_44BFC0 call site pushes four cdecl arguments (see notes), so that
// binding is widened to match the disassembly.
CSF_FUNC(int, __cdecl, sub_44C4A0, 0x0044C4A0, (int* a1));
CSF_FUNC(int, __cdecl, sub_44BFC0, 0x0044BFC0, (int a1, int a2, int a3, int a4));
CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the HUD registration and the owned block, then clears the slot.
static int __fastcall C_008311c4_Destroy(C_008311c4* self, void* /*edx*/)
{
    // Restore the primary vtable and the embedded-object vtable.
    self->vtbl = CSF_VTBL(0x008311C4);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_VT(0x008311A8);

    if (self->field_0008)
    {
        sub_44C4A0(reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x0C));
        Hud_UnregisterObject((int)(uintptr_t)self);
        self->field_0008 = 0;
    }

    int result = (int)self->field_0010;
    if (result)
    {
        sub_44BFC0(result, (int)self->field_0014,
                   (int)(uintptr_t)(reinterpret_cast<uint8_t*>(self) + 0x0C),
                   (int)(uintptr_t)self);
        result = (int)self->field_0010;
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0010));
    }

    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0044CC90, C_008311c4_Destroy, "auto");
