#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, sub_698600, 0x00698600, (char a1));

// Stores the requested scene index, raises the request flag, then dispatches
// through vtable slot 0x44 (vtbl[17]) and returns that slot's result.
static int __fastcall CSceneLoaderBase_RequestSceneLoad(C_0083f578* self, void* /*edx*/, int a2)
{
    sub_698600(0);
    CSF_AT(int, self, 0x20) = 1;
    CSF_AT(int, self, 0x14) = a2;
    return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x44 / 4])(self, nullptr);
}

HOOK(0x004FDC70, CSceneLoaderBase_RequestSceneLoad, "auto");
