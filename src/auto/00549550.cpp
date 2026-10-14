#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator the original destructor uses).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* p));

// 0x549CA0: destructor body / member teardown of the CMenuVideoOptionsInGame chain;
// runs after the vtable pointers are restored, before the optional free.
CSF_FUNC(void, __fastcall, CMenuVideoOptions_Destruct_549CA0, 0x549CA0,
         (CMenuVideoOptionsInGame* self, void* edx));

// CMenuVideoOptionsInGame destructor: restores the primary vtable (offset 0x00) and the
// secondary vtable (offset 0x04), runs the destructor body and, when the low bit of the
// flag argument is set, frees the object on the game heap. Always returns the object
// pointer in eax.
static void* __fastcall CMenuVideoOptionsInGame_Destroy(CMenuVideoOptionsInGame* self, void* /*edx*/, char a2)
{
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x008440A0);
    self->field_0004 = CSF_ADDR_VT(0x00844050);

    CMenuVideoOptions_Destruct_549CA0(self, nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x549550, CMenuVideoOptionsInGame_Destroy, "auto");
