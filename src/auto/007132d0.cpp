#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Copies a 0x100-byte input state buffer from the argument into this+0xB30.
static void __fastcall CInputDirectInput_CopyStateBuffer256(void* self, void* /*edx*/, const void* sourceBuffer)
{
    std::memcpy(static_cast<uint8_t*>(self) + 0xB30, sourceBuffer, 0x100);
}

HOOK(0x7132D0, CInputDirectInput_CopyStateBuffer256, "auto");
