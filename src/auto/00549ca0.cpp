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

// CMenuVideoOptions destructor body (scalar deleting wrapper not included here).
// Restores the CMenuVideoOptions vtable chain, releases the child object at +0x1C
// through its virtual destructor (slot 0, flag 1 = free), clears the slot, then
// rewrites the base-class vtables in the original order.
static void __fastcall CMenuVideoOptions_Destruct(CMenuVideoOptions* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_001c);

    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x00844148);   // CMenuVideoOptions vtable
    self->field_0004 = 0x008440F8;            // CMenuScreen intermediate vtable

    if (child) {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_001c = 0;
    }

    self->field_0004 = CSF_ADDR_DATA(0x00830488);
    self->field_0004 = CSF_ADDR_VT(0x00830438);
    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x0083042C);
}

HOOK(0x00549CA0, CMenuVideoOptions_Destruct, "auto");

