#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets the action-support state block to its defaults and stores a2 as the
// first dword. The original is __thiscall with one stack argument (ret 4),
// so it is written here as __fastcall with ecx = this and edx unused.
static int __fastcall Setup_InitializeActionSupport(void* self, void* /*edx*/, int a2)
{
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x20) = 0;
    CSF_AT(uint8_t, self, 0x30) = 0;
    CSF_AT(int, self, 0x04) = -1;
    CSF_AT(int, self, 0x00) = a2;
    return a2;
}

HOOK(0x0048D7B0, Setup_InitializeActionSupport, "auto");
