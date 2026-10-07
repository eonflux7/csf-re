#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// 0x00476090
// Reads the pointer member at self+0x40, then returns the dword stored at
// +0xD4 inside the object it points to.
static int __fastcall GetMemberValueD4(void* self, void* /*edx*/)
{
    void* member = reinterpret_cast<void*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x40)));
    return static_cast<int>(CSF_AT(uint32_t, member, 0xD4));
}

HOOK(0x00476090, GetMemberValueD4, "auto");
