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

// CMenuControls destructor body (scalar deleting wrapper not included here).
// Restores the CMenuControls vtable chain, releases the child object at +0x14
// through its virtual destructor (slot 0, flag 1 = free), then rewrites the
// base-class vtables in the original order.
static void __fastcall CMenuControls_Destruct(CMenuControls* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0014);

    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x008435F8);   // CMenuControls vtable
    self->field_0004 = CSF_ADDR_VT(0x008435A8);            // CMenuScreen intermediate vtable

    if (child) {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_0014 = 0;
    }

    self->field_0004 = CSF_ADDR_DATA(0x00830488);
    self->field_0004 = CSF_ADDR_VT(0x00830438);
    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x0083042C);
}

HOOK(0x00542E60, CMenuControls_Destruct, "auto");
