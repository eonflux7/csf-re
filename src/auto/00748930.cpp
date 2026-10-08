#include <cstdint>
#include "globals.h"
#include "hook.h"

// Thunk at 0x00748930 (11 bytes).
//
// The pointer arriving in ECX refers to a sub-object that is embedded at
// offset +0x308 inside its complete object. The thunk adjusts ECX to the
// owning object (`add ecx, 0xFFFFFCF8` == this - 0x308) and tail-jumps to
// the real implementation at 0x0064B250, which receives the adjusted `this`
// and pops no stack bytes of its own (rets=[0]).

CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

static int __fastcall ForwardToOwnerObjectMethod(void* self, void* /*edx*/)
{
    return Sub_64B250(reinterpret_cast<uint8_t*>(self) - 0x308, nullptr);
}

HOOK(0x00748930, ForwardToOwnerObjectMethod, "auto");
