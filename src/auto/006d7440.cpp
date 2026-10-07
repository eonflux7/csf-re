#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns a pointer to the member at offset 0x130 of the object.
static void* __fastcall C_008611d4_GetField0130(C_008611d4* self, void* /*edx*/)
{
    return reinterpret_cast<uint8_t*>(self) + 0x130;
}

HOOK(0x6D7440, C_008611d4_GetField0130, "auto");
