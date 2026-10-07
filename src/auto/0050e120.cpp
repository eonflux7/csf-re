#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_Object_GetInnerByteAt0x50(void* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x1C));
    return CSF_AT(char, inner, 0x50);
}

HOOK(0x0050E120, C_Object_GetInnerByteAt0x50, "auto");
