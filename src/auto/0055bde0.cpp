#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator used by the original (0x6910F0).
CSF_FUNC(int, __cdecl, Mem_Alloc, 0x6910F0, (size_t size));

// Vtable pointer stored at offset 0 of the created object.
CSF_GLOBAL(uint8_t, g_vtbl_8453A4, 0x8453A4);

// Allocates a 28-byte object, installs its vtable (0x8453A4) and zero/defaults its fields.
// Returns the new object, or 0 when the allocation fails.
static int __cdecl MenuMpAccounts_CreateObject8453A4()
{
    int object = Mem_Alloc(0x1C);
    if (object == 0)
        return 0;

    uint8_t* base = reinterpret_cast<uint8_t*>(object);
    base[4] = 0;
    *reinterpret_cast<void**>(base + 0) = &g_vtbl_8453A4;
    *reinterpret_cast<uint32_t*>(base + 8) = 0;
    *reinterpret_cast<uint32_t*>(base + 12) = 0;
    *reinterpret_cast<uint32_t*>(base + 16) = 0;
    *reinterpret_cast<uint32_t*>(base + 20) = 10;
    return object;
}

HOOK(0x55BDE0, MenuMpAccounts_CreateObject8453A4, "auto");
