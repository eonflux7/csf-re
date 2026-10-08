#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the object used as the root of the global state (address 0x4E8C50).
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x4E8C50, ());

// Reads the pointer stored at +0x1FD8 of the global object and returns the
// dword stored at +0x14 of that nested object.
static int __cdecl GetGlobalNestedField14()
{
    uint8_t* globalObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    uint32_t nestedObject = *reinterpret_cast<uint32_t*>(globalObject + 0x1FD8);
    return *reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(nestedObject)) + 0x14);
}

HOOK(0x44F3D0, GetGlobalNestedField14, "auto");
