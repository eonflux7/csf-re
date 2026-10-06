#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the object itself (identity / "get this" accessor).
static void* __fastcall C_008310b8_GetSelf(C_008310b8* self, void* /*edx*/)
{
    return self;
}

HOOK(0x4B2980, C_008310b8_GetSelf, "auto");
