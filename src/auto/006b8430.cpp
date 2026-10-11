#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Global table base: dword_8BB488, used as the base of a per-object property table.
CSF_GLOBAL(uint32_t, g_ObjectPropsTableBase, 0x008BB488);

// Reads the entry at (table base + index + 4), follows the pointer stored there,
// and returns the first dword of the pointed-to block.
static int __cdecl ObjectProps_ReadEntryFirstDword(int entryIndex)
{
    uint32_t entryPointer = CSF_AT(uint32_t,
                                   reinterpret_cast<uint8_t*>(g_ObjectPropsTableBase),
                                   entryIndex + 4);
    return CSF_AT(int, reinterpret_cast<uint8_t*>(entryPointer), 0);
}

HOOK(0x006B8430, ObjectProps_ReadEntryFirstDword, "auto");
