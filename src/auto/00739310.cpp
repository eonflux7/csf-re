#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00691110  int __cdecl Mem_Free(void*)
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* a1));

// 0x006923A0  void __thiscall C_0085e65c_InitVtable(C_0085e65c* this)
CSF_FUNC(void, __fastcall, C_0085e65c_InitVtable, 0x6923A0, (C_0085e65c* self, void* edx));

// Scalar deleting destructor for C_008673fc.
// - stores its own vtable pointer (0x8673FC)
// - invokes the base-class vtable init (C_0085e65c_InitVtable)
// - if the low bit of the flag argument is set, frees the object via the game heap
//   Mem_Free, then returns the pointer.
static C_008673fc* __fastcall C_008673fc_ScalarDeletingDestructor(C_008673fc* self, void* /*edx*/, char a2)
{
    self->vtbl = reinterpret_cast<void**>(0x8673FC);
    C_0085e65c_InitVtable(self, nullptr);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return self;
}

HOOK(0x739310, C_008673fc_ScalarDeletingDestructor, "auto");
