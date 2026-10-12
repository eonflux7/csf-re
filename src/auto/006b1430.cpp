#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Copies a 0x38-byte fog parameter block from the argument into this+0x18.
static void __fastcall SceneLoader_SetFogParameters(void* self, void* /*edx*/, const void* sourceParams)
{
    std::memcpy(static_cast<uint8_t*>(self) + 0x18, sourceParams, 0x38);
}

HOOK(0x6B1430, SceneLoader_SetFogParameters, "auto");
