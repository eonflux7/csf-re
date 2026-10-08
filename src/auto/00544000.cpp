#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base destructor at 0x53B5A0, reached by tail jump (ret 0, __thiscall, no stack args).
CSF_FUNC(int, __fastcall, CMenuGameOptions_Destruct_53B5A0, 0x53B5A0, (void* self, void* edx));

static int __fastcall CMenuGameOptionsPC_Destroy(CMenuGameOptionsPC* self, void* edx)
{
    // Install primary vtable pointer (offset 0, not a declared field).
    CSF_AT(uint32_t, self, 0x0) = CSF_ADDR_VT(0x008439B8);
    // Install secondary/base vtable pointer kept in CMenuScreen::field_0004.
    self->field_0004 = 0x00843968u;
    // Tail-call the base destructor, forwarding ecx/edx unchanged.
    return CMenuGameOptions_Destruct_53B5A0(self, edx);
}

HOOK(0x544000, CMenuGameOptionsPC_Destroy, "auto");
