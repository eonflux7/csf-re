#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Calls the virtual at vtable slot 0xa4/4 on `self` and returns the dword
// stored 4 bytes into the object it returns.
static int __fastcall C_00866db8_GetLinkedField4(C_00866db8* self, void* /*edx*/)
{
    void* linked = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xa4 / 4])(self, nullptr);
    return *reinterpret_cast<int*>(static_cast<uint8_t*>(linked) + 4);
}

HOOK(0x00730800, C_00866db8_GetLinkedField4, "auto");
