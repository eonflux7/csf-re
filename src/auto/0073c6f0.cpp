#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets the byte flag at offset 0x14E to 1. The single stack argument is ignored.
static void __fastcall SetFlagAt14E(uint8_t* self, void* /*edx*/, int /*unusedArg*/)
{
    CSF_AT(uint8_t, self, 0x14E) = 1;
}

HOOK(0x0073C6F0, SetFlagAt14E, "auto");
