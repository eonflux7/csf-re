#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns a pointer to the embedded sub-object / field at offset 0x84 of this.
static uint8_t* __fastcall C_00867260_GetField0084(C_00867260* self, void* /*edx*/)
{
    return reinterpret_cast<uint8_t*>(self) + 0x84;
}

HOOK(0x48E450, C_00867260_GetField0084, "auto");
