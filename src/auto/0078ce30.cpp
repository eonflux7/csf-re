#include <cstdint>
#include "hook.h"
#include "globals.h"

// The object's first dword is its vtable pointer; the vtable itself lives at 0x00872CA4.
CSF_GLOBAL(void*, g_vtbl_872CA4, 0x872CA4);

// 0x0078CE30 (7 bytes) - writes the vtable pointer (&0x00872CA4) into the object's first dword.
// __thiscall with no stack arguments -> callee pops 0 bytes.
static void __fastcall Sub_78CE30_SetVtable(void** self, void* /*edx*/)
{
    *self = &g_vtbl_872CA4;
}

HOOK(0x78CE30, Sub_78CE30_SetVtable, "auto");
