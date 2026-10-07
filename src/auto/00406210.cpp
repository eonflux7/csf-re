#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Stores the passed value at offset +0x8 of the object and returns 1.
static char __fastcall SetValueAtOffset8(void* self, void* /*edx*/, int a2)
{
    CSF_AT(int, self, 0x8) = a2;
    return 1;
}

HOOK(0x406210, SetValueAtOffset8, "auto");
