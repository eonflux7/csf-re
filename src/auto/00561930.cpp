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

// CMenuMpSessions destructor body (scalar deleting wrapper not included here).
// Restores the CMenuMpSessions vtable chain, releases the child object at +0x40
// through its virtual destructor (slot 0, flag 1 = free), then rewrites the
// base-class vtables in the original order.
static void __fastcall CMenuMpSessions_Destruct(CMenuMpSessions* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0040);

    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x00845990);   // CMenuMpSessions vtable
    self->field_0004 = CSF_ADDR_VT(0x00845940);            // CMenuScreen intermediate vtable

    if (child) {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_0040 = 0;
    }

    self->field_0004 = CSF_ADDR_DATA(0x00830488);
    self->field_0004 = CSF_ADDR_VT(0x00830438);
    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x0083042C);
}

HOOK(0x00561930, CMenuMpSessions_Destruct, "auto");
