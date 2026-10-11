// Calls its own slot +0x2C, then forwards the call to its slot +0x60 (0x00523760).
#include <cstdint>
#include "globals.h"
#include "hook.h"

static hook::Tail __fastcall C_00840ff0_CallSlot2CAndForwardSlot60(void* self, const uint32_t* /*args*/)
{
    (*reinterpret_cast<int(__fastcall***)(void*, void*)>(self))[0x2C / 4](self, nullptr);
    return hook::tail_jump(self, (*reinterpret_cast<void***>(self))[0x60 / 4]);
}

HOOK_TAIL(0x523760, C_00840ff0_CallSlot2CAndForwardSlot60, "auto");
