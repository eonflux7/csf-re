#include <cstdint>

#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Reads the pointer held at +0x40 (an owner/manager object) off `self`, then
// returns the 32-bit value stored at +0x104 of that owner object.
static int __fastcall GetOwnerValueAt0x104(void* self, void* /*edx*/)
{
    void* owner = *CSF_AT(void**, self, 0x40);
    return *CSF_AT(int*, owner, 0x104);
}

HOOK(0x00476070, GetOwnerValueAt0x104, "auto");
