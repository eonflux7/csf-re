#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_008313f8*, __fastcall, AssignEntry, 0x00472B60, (C_008313f8* self, void*, int sourceEntry));
static uint32_t __cdecl Assign(hook::Regs* regs, const uint32_t* args) {
    uint32_t result = regs->eax; // Empty range does not write EAX.
    for (uint32_t cursor = args[0]; cursor != args[1]; cursor += 12)
        result = reinterpret_cast<uint32_t>(AssignEntry(reinterpret_cast<C_008313f8*>(cursor), nullptr, args[2]));
    return result;
}
HOOK_REGS(0x0046EDA0, Assign, 0, "stl");
