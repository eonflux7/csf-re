#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00713FB0
// String-data accessor with small-string optimization:
//   if capacity/length field_0028 < 0x10 the characters live inline at +0x14,
//   otherwise field_0014 holds a pointer to the heap buffer.
static void* __fastcall C_008656d0_GetDataPointer(C_008656d0* self, void* /*edx*/)
{
    if (self->field_0028 < 0x10u)
        return reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x14);
    return reinterpret_cast<void*>(self->field_0014);
}

HOOK(0x713FB0, C_008656d0_GetDataPointer, "auto");
