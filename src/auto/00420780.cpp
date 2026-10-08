#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the static type-name string "TAnimacion" (stored at 0x82F618).
static const char* __cdecl GetTAnimacionName()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x82F618));
}

HOOK(0x420780, GetTAnimacionName, "auto");
