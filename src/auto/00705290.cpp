#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the allocator the original uses for its optional free).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// 0x6B2830: base-class constructor of C_00860170, runs after the vtable store and before the optional free.
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// C_008631a8 constructor: installs its vtable, clears field_0010, runs the base constructor and,
// when the low bit of the flag argument is set, frees the object on the game heap.
// Returns the object pointer.
static C_008631a8* __fastcall C_008631a8_Construct(C_008631a8* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x008631A8);
    self->field_0010 = 0;

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return self;
}

HOOK(0x00705290, C_008631a8_Construct, "auto");

