#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0064B250: real implementation, receives the base object in ecx and pops no stack bytes.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

// Thunk: this is a secondary-base pointer; the real object starts 0xD8 bytes earlier.
// Rewrites ecx to the base object address and tail-calls the real implementation.
static int __fastcall CallBaseMethod64B250(void* self, void* /*edx*/)
{
    return Sub_64B250(reinterpret_cast<uint8_t*>(self) - 0xD8, nullptr);
}

HOOK(0x00753310, CallBaseMethod64B250, "auto");
