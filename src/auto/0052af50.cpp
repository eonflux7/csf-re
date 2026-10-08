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

// Callees, bound by address.
CSF_FUNC(int, __fastcall, CMenuMessageBox_DestroyOwnedObject, 0x528A20, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, MenuBuffer_Destroy, 0x4FF3C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Base10_Destroy, 0x69AE90, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Mem_Free, 0x691110, (void* block));

// __thiscall, ret 0: destructor body. Vtable pointers are written as the
// original's immediate addresses, in the same order as the original.
static int __fastcall CMenuMessageBox_DestructBody(CMenuMessageBox* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00841370);
    self->field_0004 = 0x00841320u;
    CSF_AT(uint32_t, self, 0x10) = 0x00841318u;

    CMenuMessageBox_DestroyOwnedObject(self, nullptr);

    if (self->field_0954 != 0)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0954));
        self->field_0954 = 0;
    }

    MenuBuffer_Destroy(reinterpret_cast<uint8_t*>(self) + 0x8A0, nullptr);
    int result = Base10_Destroy(reinterpret_cast<uint8_t*>(self) + 0x10, nullptr);

    self->field_0004 = CSF_ADDR_DATA(0x00830488);
    self->field_0004 = CSF_ADDR_VT(0x00830438);
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x0083042C);
    return result;
}

HOOK(0x0052AF50, CMenuMessageBox_DestructBody, "auto");
