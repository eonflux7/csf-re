#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, WaterShader_ProcessSubObject, 0x0070BDF0, (void* self, void* edx, char a2));

static uint32_t* __fastcall WaterShader_ForwardFromSubObject(void* self, void* /*edx*/, char a2)
{
    // The original subtracts 4 from `this` (sub ecx, 4) and tail-jumps with the
    // stack argument forwarded unchanged.
    C_0082d410* subObject = reinterpret_cast<C_0082d410*>(static_cast<uint8_t*>(self) - 4);
    return WaterShader_ProcessSubObject(subObject, nullptr, a2);
}

HOOK(0x0070BE20, WaterShader_ForwardFromSubObject, "auto");
