#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0064B250 — tail-call target of the thunk; reads its `this` from ecx (__thiscall, no stack args).
CSF_FUNC(int, __fastcall, Sub_64B250, 0x64B250, (void* self, void* edx));

// 0x00749380 — adjustor thunk: rebases the incoming this-pointer by -0x440 and tail-calls 0x64B250.
static int __fastcall AdjustThis_Minus440_To_64B250(void* self, void* /*edx*/)
{
    return Sub_64B250(reinterpret_cast<uint8_t*>(self) - 0x440, nullptr);
}

HOOK(0x749380, AdjustThis_Minus440_To_64B250, "auto");
