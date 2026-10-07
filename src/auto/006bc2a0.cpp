#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the dword stored at object offset 0x00A8 and returns it.
// The incoming stack argument is ignored.
static int __fastcall GetMemberAtA8(void* self, void* /*edx*/, int /*ignored*/)
{
    return CSF_AT(int, self, 0xa8);
}

HOOK(0x006BC2A0, GetMemberAtA8, "auto");
