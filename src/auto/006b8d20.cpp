#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Global table base used to index per-object property slots (dword_8BB488).
CSF_GLOBAL(uint32_t, g_ObjectPropsTableBase, 0x008BB488);

// Returns the size of an object's property record: 12 plus the 16-bit value stored at
// +0x84 when that value is nonzero and the object's table entry points to a non-zero
// first dword; otherwise 12.
static int __cdecl ObjectProps_ComputeRecordSize(int objectAddress)
{
    uint16_t propertyWordValue = CSF_AT(uint16_t, objectAddress, 0x84);

    // Entry = *(uint32_t*)(tableBase + objectAddress + 4); then deref its first dword.
    uintptr_t entryAddress = static_cast<uintptr_t>(g_ObjectPropsTableBase) + static_cast<uintptr_t>(objectAddress) + 4;
    uint32_t entryPointer = *reinterpret_cast<uint32_t*>(entryAddress);
    uint32_t entryFirstDword = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryPointer));

    if (entryFirstDword != 0 && propertyWordValue != 0)
        return static_cast<int>(propertyWordValue) + 12;
    return 12;
}

HOOK(0x006B8D20, ObjectProps_ComputeRecordSize, "auto");
