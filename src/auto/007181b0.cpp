#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the embedded sub-object located at this + 0xB8.
// (offset 0xB8 falls inside the base's pad_00a8[0x40] region, hence the raw pointer arithmetic)
static void* __fastcall C_00864288_GetEmbeddedObject(C_00864288* self, void* /*edx*/)
{
    return reinterpret_cast<uint8_t*>(self) + 0xb8;
}

HOOK(0x007181B0, C_00864288_GetEmbeddedObject, "auto");
