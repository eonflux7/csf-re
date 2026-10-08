#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0064B250: __thiscall method with no stack arguments (ret 0).
CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

// Thunk at 0x00743760: adjusts `this` down to the embedded base sub-object
// located at (this - 0xB8) and tail-calls the method at 0x0064B250 on it.
static int __fastcall Call64B250OnBaseSubobject(void* self, void* /*edx*/)
{
    return Sub_64B250(reinterpret_cast<uint8_t*>(self) - 0xB8, nullptr);
}

HOOK(0x743760, Call64B250OnBaseSubobject, "auto");
