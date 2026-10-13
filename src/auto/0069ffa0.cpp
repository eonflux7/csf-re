#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the object pointer stored at a1+0x40, calls its vtable slot 6
// (offset 0x18) as a __thiscall with no arguments, and returns the
// result as a 0/1 int.
static int __cdecl RtFS_IsBackendReady(int a1)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(a1);
    void* backend = CSF_AT(void*, base, 0x40);
    uint8_t status = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, backend, 0)[0x18 / 4])(backend, nullptr);
    return status != 0 ? 1 : 0;
}

HOOK(0x69FFA0, RtFS_IsBackendReady, "auto");
