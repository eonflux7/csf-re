// Clears the field at +0x64, calls its own slot +0x2C, then forwards the call to its slot +0x60 (0x00527510).
#include <cstdint>
#include "globals.h"
#include "hook.h"

static hook::Tail __fastcall CSoundInstance_ResetField64AndForwardSlot60(void* self, const uint32_t* /*args*/)
{
    CSF_AT(uint32_t, self, 0x64) = 0;
    (*reinterpret_cast<int(__fastcall***)(void*, void*)>(self))[0x2C / 4](self, nullptr);
    return hook::tail_jump(self, (*reinterpret_cast<void***>(self))[0x60 / 4]);
}

HOOK_TAIL(0x527510, CSoundInstance_ResetField64AndForwardSlot60, "auto");
