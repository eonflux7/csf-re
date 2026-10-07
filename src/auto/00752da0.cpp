#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00752DA0
// Sets the byte flag at +0x19E to 1 and returns 1 (al holds both the stored value
// and the return value). __thiscall, no stack arguments (ret 0).
static char __fastcall MarkActiveAndReturnTrue(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x19e) = 1;
    return 1;
}

HOOK(0x752DA0, MarkActiveAndReturnTrue, "auto");
