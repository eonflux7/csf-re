#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x006FE3D0: trivial byte setter.
// Stores the incoming byte argument at this+0x22 and returns it unchanged.
static char __fastcall SetByteProperty22(void* self, void* /*edx*/, char value)
{
    CSF_AT(char, self, 0x22) = value;
    return value;
}

HOOK(0x6FE3D0, SetByteProperty22, "auto");
